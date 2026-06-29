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
    preprocessor_.define_macro("true", "1");
    preprocessor_.define_macro("false", "0");
    preprocessor_.define_macro("__bool_true_false_are_defined", "1");
}

CompileResult Compiler::compile(const std::string& source) {
    CompileResult result;
    
    // Reset state for this compilation unit
    preprocessor_.reset();
    semantic_.reset();
    // Re-define built-in macros after reset
    preprocessor_.define_macro("__STDC__", "1");
    preprocessor_.define_macro("__STDC_VERSION__", "202311L");
    preprocessor_.define_macro("__x86_64__", "1");
    preprocessor_.define_macro("__linux__", "1");
    preprocessor_.define_macro("true", "1");
    preprocessor_.define_macro("false", "0");
    preprocessor_.define_macro("__bool_true_false_are_defined", "1");
    
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

MultiFileCompileResult Compiler::compile_files(const std::vector<std::string>& filenames) {
    MultiFileCompileResult result;
    result.success = true;
    
    std::string combined_assembly;
    
    for (const auto& filename : filenames) {
        auto file_result = compile_file(filename);
        
        if (!file_result.success) {
            result.success = false;
            result.error_message = file_result.error_message;
            result.error_filename = filename;
            result.error_line = file_result.error_line;
            return result;
        }
        
        if (!combined_assembly.empty()) {
            combined_assembly += "\n";
        }
        combined_assembly += file_result.assembly;
    }
    
    result.combined_assembly = combined_assembly;
    return result;
}

} // namespace simplecc
