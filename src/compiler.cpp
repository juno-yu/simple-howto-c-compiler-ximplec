#include "compiler.h"
#include "lexer.h"
#include "parser.h"
#include "codegen.h"
#include <fstream>
#include <sstream>

namespace simplecc {

Compiler::Compiler() {
    // Define built-in macros
    preprocessor_.define_macro("__STDC__", "1");
    preprocessor_.define_macro("__STDC_VERSION__", "202311L");
    preprocessor_.define_macro("__x86_64__", "1");
    preprocessor_.define_macro("__linux__", "1");
}

CompileResult Compiler::compile(const std::string& source) {
    CompileResult result;
    
    // Preprocess
    std::string preprocessed = preprocessor_.process(source);
    
    if (preprocessor_.has_error()) {
        result.error_message = "Preprocessor error: " + preprocessor_.error_message();
        result.error_line = preprocessor_.error_line();
        return result;
    }
    
    // Tokenize
    Lexer lexer(preprocessed);
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
    
    // Semantic analysis (optional, don't fail on warnings)
    semantic_.analyze(static_cast<ProgramNode&>(*ast));
    
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
