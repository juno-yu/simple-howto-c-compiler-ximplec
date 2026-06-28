#pragma once

#include <string>
#include <vector>
#include <map>

namespace simplecc {

struct Macro {
    std::string name;
    std::vector<std::string> params; // empty for object-like macros
    std::string body;
    bool is_function_like;
    bool is_variadic; // has ... parameter
    
    Macro() : is_function_like(false), is_variadic(false) {}
    Macro(const std::string& n, const std::string& b, bool fl = false, bool var = false)
        : name(n), body(b), is_function_like(fl), is_variadic(var) {}
};

class Preprocessor {
public:
    Preprocessor();
    ~Preprocessor() = default;
    
    std::string process(const std::string& source, const std::string& filename = "<stdin>");
    
    bool has_error() const { return !error_message_.empty(); }
    const std::string& error_message() const { return error_message_; }
    int error_line() const { return error_line_; }
    
    // Define a macro externally
    void define_macro(const std::string& name, const std::string& body);
    
    // Check if a macro is defined
    bool is_defined(const std::string& name) const;
    
private:
    std::string error_message_;
    int error_line_;
    
    // Macros
    std::map<std::string, Macro> macros_;
    
    // Conditional compilation stack
    std::vector<bool> condition_stack_;
    std::vector<bool> was_true_stack_; // track if any branch was taken
    
    // Include guard tracking
    std::map<std::string, bool> included_files_;
    
    // Processing methods
    std::string process_line(const std::string& line);
    std::string expand_macros(const std::string& text);
    std::string expand_macro_call(const Macro& macro, const std::vector<std::string>& args);
    
    // Directive handlers
    void handle_include(const std::string& args);
    void handle_define(const std::string& args);
    void handle_undef(const std::string& args);
    void handle_ifdef(const std::string& args);
    void handle_ifndef(const std::string& args);
    void handle_if(const std::string& args);
    void handle_else();
    void handle_endif();
    void handle_error(const std::string& args);
    void handle_pragma(const std::string& args);
    std::string handle_embed(const std::string& args);
    
    // Utility
    std::string read_file(const std::string& filename);
    std::vector<std::string> split_args(const std::string& args);
    bool evaluate_condition(const std::string& condition);
};

} // namespace simplecc
