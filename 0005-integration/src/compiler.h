#pragma once

#include "ast.h"
#include <string>

namespace simplecc {

struct CompileResult {
    bool success;
    std::string assembly;
    std::string error_message;
    int error_line;
    int error_column;
    
    CompileResult() : success(false), error_line(0), error_column(0) {}
};

class Compiler {
public:
    Compiler() = default;
    ~Compiler() = default;
    
    CompileResult compile(const std::string& source);
    CompileResult compile_file(const std::string& filename);
    
    bool has_error() const { return !error_message_.empty(); }
    const std::string& error_message() const { return error_message_; }
    
private:
    std::string error_message_;
};

} // namespace simplecc
