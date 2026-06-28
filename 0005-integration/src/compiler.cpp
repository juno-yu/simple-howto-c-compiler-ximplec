#include "compiler.h"
#include "lexer.h"
#include "parser.h"
#include "codegen.h"
#include <fstream>
#include <sstream>

namespace simplecc {

CompileResult Compiler::compile(const std::string& source) {
    CompileResult result;
    
    // Tokenize
    Lexer lexer(source);
    auto tokens = lexer.tokenize();
    
    if (lexer.has_error()) {
        result.error_message = "Lexer error: " + lexer.error_message();
        result.error_line = lexer.error_line();
        result.error_column = lexer.error_column();
        return result;
    }
    
    // Parse
    Parser parser(tokens);
    auto ast = parser.parse();
    
    if (parser.has_error()) {
        result.error_message = "Parser error: " + parser.error_message();
        result.error_line = parser.error_line();
        result.error_column = parser.error_column();
        return result;
    }
    
    // Generate code
    CodeGenerator codegen;
    result.assembly = codegen.generate(static_cast<ProgramNode&>(*ast));
    
    if (codegen.has_error()) {
        result.error_message = "Code generation error: " + codegen.error_message();
        return result;
    }
    
    result.success = true;
    return result;
}

CompileResult Compiler::compile_file(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        CompileResult result;
        result.error_message = "Could not open file: " + filename;
        return result;
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    
    return compile(buffer.str());
}

} // namespace simplecc
