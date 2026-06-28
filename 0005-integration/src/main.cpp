#include "compiler.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

void print_usage() {
    std::cerr << "Usage: simplecc [options] <file.c>" << std::endl;
    std::cerr << "Options:" << std::endl;
    std::cerr << "  -o <file>    Output file (default: a.out)" << std::endl;
    std::cerr << "  -S           Output assembly only" << std::endl;
    std::cerr << "  -t           Print tokens" << std::endl;
    std::cerr << "  -a           Print AST" << std::endl;
    std::cerr << "  -h           Show help" << std::endl;
}

int main(int argc, char* argv[]) {
    std::string input_file;
    std::string output_file = "a.out";
    bool assembly_only = false;
    bool print_tokens = false;
    bool print_ast = false;
    
    std::vector<std::string> args;
    for (int i = 1; i < argc; i++) {
        args.push_back(argv[i]);
    }
    
    for (size_t i = 0; i < args.size(); i++) {
        if (args[i] == "-h" || args[i] == "--help") {
            print_usage();
            return 0;
        } else if (args[i] == "-o") {
            if (i + 1 < args.size()) {
                output_file = args[++i];
            } else {
                std::cerr << "Error: -o requires an argument" << std::endl;
                return 1;
            }
        } else if (args[i] == "-S") {
            assembly_only = true;
        } else if (args[i] == "-t") {
            print_tokens = true;
        } else if (args[i] == "-a") {
            print_ast = true;
        } else if (args[i][0] != '-') {
            input_file = args[i];
        } else {
            std::cerr << "Unknown option: " << args[i] << std::endl;
            print_usage();
            return 1;
        }
    }
    
    if (input_file.empty()) {
        std::cerr << "Error: No input file specified" << std::endl;
        print_usage();
        return 1;
    }
    
    simplecc::Compiler compiler;
    auto result = compiler.compile_file(input_file);
    
    if (!result.success) {
        std::cerr << result.error_message << std::endl;
        return 1;
    }
    
    if (assembly_only) {
        std::cout << result.assembly;
    } else {
        // Write assembly to temp file and assemble
        std::string asm_file = input_file + ".s";
        std::ofstream out(asm_file);
        out << result.assembly;
        out.close();
        
        std::cout << "Assembly written to " << asm_file << std::endl;
        std::cout << "To assemble and link:" << std::endl;
        std::cout << "  as " << asm_file << " -o " << input_file << ".o" << std::endl;
        std::cout << "  ld " << input_file << ".o -o " << output_file << std::endl;
    }
    
    return 0;
}
