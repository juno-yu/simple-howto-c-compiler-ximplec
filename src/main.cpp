#include "compiler.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

void print_usage() {
    std::cerr << "Usage: simplecc [options] <file.c> [file2.c ...]" << std::endl;
    std::cerr << "Options:" << std::endl;
    std::cerr << "  -o <file>    Output file (default: a.out)" << std::endl;
    std::cerr << "  -S           Output assembly only" << std::endl;
    std::cerr << "  -t           Print tokens" << std::endl;
    std::cerr << "  -a           Print AST" << std::endl;
    std::cerr << "  -h           Show help" << std::endl;
}

int main(int argc, char* argv[]) {
    std::vector<std::string> input_files;
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
            input_files.push_back(args[i]);
        } else {
            std::cerr << "Unknown option: " << args[i] << std::endl;
            print_usage();
            return 1;
        }
    }
    
    if (input_files.empty()) {
        std::cerr << "Error: No input file specified" << std::endl;
        print_usage();
        return 1;
    }
    
    simplecc::Compiler compiler;
    
    if (assembly_only) {
        std::vector<std::pair<std::string, std::string>> compiled_files; // (filename, assembly)
        
        for (const auto& input_file : input_files) {
            auto result = compiler.compile_file(input_file);
            
            if (!result.success) {
                std::cerr << input_file << ": " << result.error_message << std::endl;
                return 1;
            }
            
            compiled_files.push_back({input_file, result.assembly});
        }
        
        if (input_files.size() == 1 && output_file == "a.out") {
            // Single file, no -o: print to stdout
            std::cout << compiled_files[0].second;
        } else if (input_files.size() > 1 && output_file == "a.out") {
            // Multiple files, no -o: write individual .s files
            for (const auto& [filename, assembly] : compiled_files) {
                std::string asm_file = filename + ".s";
                std::ofstream out(asm_file);
                out << assembly;
                out.close();
                
                std::cout << "Assembly written to " << asm_file << std::endl;
            }
        } else {
            // -o specified: write combined assembly to output file
            std::string combined_assembly;
            for (const auto& [filename, assembly] : compiled_files) {
                combined_assembly += assembly;
                combined_assembly += "\n";
            }
            
            std::ofstream out(output_file);
            out << combined_assembly;
            out.close();
            std::cout << "Assembly written to " << output_file << std::endl;
        }
    } else {
        for (const auto& input_file : input_files) {
            auto result = compiler.compile_file(input_file);
            
            if (!result.success) {
                std::cerr << input_file << ": " << result.error_message << std::endl;
                return 1;
            }
            
            std::string asm_file = input_file + ".s";
            std::ofstream out(asm_file);
            out << result.assembly;
            out.close();
            
            std::cout << "Assembly written to " << asm_file << std::endl;
        }
        
        std::cout << "To assemble and link:" << std::endl;
        std::cout << "  as ";
        for (const auto& input_file : input_files) {
            std::cout << input_file << ".s ";
        }
        std::cout << "-o " << output_file << std::endl;
    }
    
    return 0;
}
