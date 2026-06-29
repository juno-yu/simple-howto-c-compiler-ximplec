#pragma once

#include "compiler.h"
#include <string>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdexcept>

namespace simplecc {

// Result of a compile attempt with a process-level timeout.
struct TimedCompileResult {
    bool success;          // true if compile() returned success=true
    bool timed_out;        // true if child process was killed by timeout
    bool threw_bad_alloc;  // true if child process threw std::bad_alloc
    int  signal;           // signal that killed the child, 0 if none
    std::string error_message;
    std::string assembly;

    TimedCompileResult()
        : success(false), timed_out(false), threw_bad_alloc(false), signal(0) {}
};

// Run compile() in a child process so that an infinite loop in the
// parser/codegen (e.g., the `%` and `>>` bugs) does not hang the test
// suite.  Kills the child after `timeout_seconds`.
inline TimedCompileResult compile_with_timeout(const std::string& src,
                                               int timeout_seconds = 2) {
    TimedCompileResult out;
    int pipefd[2];
    if (::pipe(pipefd) != 0) {
        out.error_message = "pipe() failed";
        return out;
    }

    pid_t pid = ::fork();
    if (pid < 0) {
        ::close(pipefd[0]); ::close(pipefd[1]);
        out.error_message = "fork() failed";
        return out;
    }

    if (pid == 0) {
        // Child: do the compile and write a one-character status byte
        // followed by serialized result fields.
        ::close(pipefd[0]);
        char status = '?';
        try {
            Compiler c;
            CompileResult r = c.compile(src);
            if (r.success) status = 'O';
            else status = 'E';
            // Write status
            ::write(pipefd[1], &status, 1);
            // Write assembly length (4 bytes) then assembly
            uint32_t len = (uint32_t)r.assembly.size();
            ::write(pipefd[1], &len, sizeof(len));
            if (len > 0) ::write(pipefd[1], r.assembly.data(), len);
            // Write error_message length then error_message
            uint32_t mlen = (uint32_t)r.error_message.size();
            ::write(pipefd[1], &mlen, sizeof(mlen));
            if (mlen > 0) ::write(pipefd[1], r.error_message.data(), mlen);
        } catch (const std::bad_alloc&) {
            status = 'B';
            ::write(pipefd[1], &status, 1);
        } catch (const std::exception& e) {
            status = 'X';
            ::write(pipefd[1], &status, 1);
            uint32_t mlen = (uint32_t)std::string(e.what()).size();
            ::write(pipefd[1], &mlen, sizeof(mlen));
            if (mlen > 0) ::write(pipefd[1], e.what(), mlen);
        } catch (...) {
            status = '?';
            ::write(pipefd[1], &status, 1);
        }
        ::close(pipefd[1]);
        ::_exit(0);
    }

    // Parent: read the status and result, with a timeout.
    ::close(pipefd[1]);
    int status = 0;
    char sbyte = 0;

    // Poll for child completion.
    bool child_done = false;
    for (int i = 0; i < timeout_seconds * 10; i++) {
        ::usleep(100000); // 100ms
        pid_t r = ::waitpid(pid, &status, WNOHANG);
        if (r == pid) { child_done = true; break; }
    }

    if (!child_done) {
        // Timed out: kill child.
        ::kill(pid, SIGKILL);
        ::waitpid(pid, &status, 0);
        ::close(pipefd[0]);
        out.timed_out = true;
        out.error_message = "compile timed out";
        return out;
    }

    // Read the result bytes (best effort - we may not have time before signal).
    ssize_t n = ::read(pipefd[0], &sbyte, 1);
    if (n != 1) {
        ::close(pipefd[0]);
        if (WIFSIGNALED(status)) {
            out.signal = WTERMSIG(status);
            out.error_message = "child killed by signal";
        } else {
            out.error_message = "child exited without writing result";
        }
        return out;
    }

    if (sbyte == 'O' || sbyte == 'E') {
        uint32_t len = 0;
        // Read assembly
        if (::read(pipefd[0], &len, sizeof(len)) == sizeof(len)) {
            out.assembly.resize(len);
            if (len > 0) ::read(pipefd[0], &out.assembly[0], len);
        }
        // Read error_message
        if (::read(pipefd[0], &len, sizeof(len)) == sizeof(len)) {
            out.error_message.resize(len);
            if (len > 0) ::read(pipefd[0], &out.error_message[0], len);
        }
        out.success = (sbyte == 'O');
    } else if (sbyte == 'B') {
        out.threw_bad_alloc = true;
        out.error_message = "child threw std::bad_alloc";
    } else if (sbyte == 'X') {
        uint32_t len = 0;
        if (::read(pipefd[0], &len, sizeof(len)) == sizeof(len)) {
            std::string m;
            m.resize(len);
            if (len > 0) ::read(pipefd[0], &m[0], len);
            out.error_message = "child threw exception: " + m;
        }
    } else {
        out.error_message = "child returned unknown status";
    }
    ::close(pipefd[0]);
    return out;
}

} // namespace simplecc
