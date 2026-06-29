#pragma once

#include "ast.h"
#include "preprocessor.h"
#include "semantic.h"
#include <string>
#include <vector>

namespace simplecc {

struct CompileResult {
    bool success;
    std::string assembly;
    std::string error_message;
    int error_line;
    int error_column;
    
    CompileResult() : success(false), error_line(0), error_column(0) {}
};

// Result of compiling multiple files together
struct MultiFileCompileResult {
    bool success;
    std::string combined_assembly;
    std::string error_message;
    std::string error_filename;
    int error_line;
    
    MultiFileCompileResult() : success(false), error_line(0) {}
};

class Compiler {
public:
    Compiler();
    ~Compiler() = default;
    
    CompileResult compile(const std::string& source);
    CompileResult compile_file(const std::string& filename);
    
    // Compile multiple files and combine their assembly output
    MultiFileCompileResult compile_files(const std::vector<std::string>& filenames);
    
    bool has_error() const { return !error_message_.empty(); }
    const std::string& error_message() const { return error_message_; }
    
    // Access preprocessor for macro definitions
    Preprocessor& preprocessor() { return preprocessor_; }
    
private:
    std::string error_message_;
    Preprocessor preprocessor_;
    SemanticAnalyzer semantic_;
};

} // namespace simplecc
