#include "preprocessor.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

namespace simplecc {

Preprocessor::Preprocessor() : error_line_(0) {}

void Preprocessor::reset() {
    error_message_.clear();
    error_line_ = 0;
    macros_.clear();
    condition_stack_.clear();
    was_true_stack_.clear();
    included_files_.clear();
}

std::string Preprocessor::process(const std::string& source, const std::string& filename) {
    error_message_.clear();
    error_line_ = 0;
    
    // Define built-in macros
    macros_["__STDC__"] = Macro("__STDC__", "1");
    macros_["__STDC_VERSION__"] = Macro("__STDC_VERSION__", "202311L");
    macros_["__x86_64__"] = Macro("__x86_64__", "1");
    macros_["__linux__"] = Macro("__linux__", "1");
    macros_["NULL"] = Macro("NULL", "0");
    macros_["true"] = Macro("true", "1");
    macros_["false"] = Macro("false", "0");
    macros_["__bool_true_false_are_defined"] = Macro("__bool_true_false_are_defined", "1");
    
    std::istringstream stream(source);
    std::string line;
    std::string output;
    int line_num = 0;
    bool skipping = false; // true when inside #if/#ifdef that evaluated false
    
    while (std::getline(stream, line)) {
        line_num++;

        // Check if line starts with #
        std::string trimmed = line;
        size_t pos = trimmed.find_first_not_of(" \t");
        if (pos == std::string::npos) {
            output += line + "\n";
            continue;
        }
        trimmed = trimmed.substr(pos);
        
        if (trimmed[0] == '#') {
            // Parse directive
            std::string directive = trimmed.substr(1);
            size_t space = directive.find_first_of(" \t");
            std::string dir_name, dir_args;
            
            if (space != std::string::npos) {
                dir_name = directive.substr(0, space);
                dir_args = directive.substr(space + 1);
                // Trim leading whitespace from args
                size_t start = dir_args.find_first_not_of(" \t");
                if (start != std::string::npos) dir_args = dir_args.substr(start);
            } else {
                dir_name = directive;
            }
            
            // Handle conditional compilation
            if (dir_name == "ifdef") {
                handle_ifdef(dir_args);
                skipping = !condition_stack_.empty() && !condition_stack_.back();
                continue;
            } else if (dir_name == "ifndef") {
                handle_ifndef(dir_args);
                skipping = !condition_stack_.empty() && !condition_stack_.back();
                continue;
            } else if (dir_name == "if") {
                handle_if(dir_args);
                skipping = !condition_stack_.empty() && !condition_stack_.back();
                continue;
            } else if (dir_name == "else") {
                handle_else();
                skipping = !condition_stack_.empty() && !condition_stack_.back();
                continue;
            } else if (dir_name == "endif") {
                handle_endif();
                skipping = !condition_stack_.empty() && !condition_stack_.back();
                continue;
            }
            
            // Skip other directives if we're in a false branch
            if (skipping) continue;
            
            // Handle other directives
            if (dir_name == "include") {
                handle_include(dir_args);
                if (has_error()) return "";
                // Read included file content and insert it
                std::string filename;
                bool is_angle_bracket = false;
                size_t start = dir_args.find_first_not_of(" \t");
                if (start != std::string::npos) {
                    if (dir_args[start] == '"') {
                        size_t end = dir_args.find('"', start + 1);
                        if (end != std::string::npos) filename = dir_args.substr(start + 1, end - start - 1);
                    } else if (dir_args[start] == '<') {
                        size_t end = dir_args.find('>', start + 1);
                        if (end != std::string::npos) filename = dir_args.substr(start + 1, end - start - 1);
                        is_angle_bracket = true;
                    }
                }
                if (!filename.empty()) {
                    std::string content;
                    // Try direct path first
                    content = read_file(filename);
                    // For angle-bracket includes, try lib/ directory
                    if (is_angle_bracket && has_error()) {
                        error_message_.clear();
                        content = read_file("lib/" + filename);
                    }
                    // For quote includes, also try lib/ as fallback
                    if (!has_error() && content.empty() && !is_angle_bracket) {
                        content = read_file("lib/" + filename);
                    }
                    if (!has_error()) {
                        // Process included content recursively
                        Preprocessor inner;
                        std::string processed = inner.process(content, filename);
                        output += processed + "\n";
                    }
                } else {
                    output += "/* #include " + dir_args + " */\n";
                }
            } else if (dir_name == "define") {
                handle_define(dir_args);
            } else if (dir_name == "undef") {
                handle_undef(dir_args);
            } else if (dir_name == "error") {
                handle_error(dir_args);
                return "";
            } else if (dir_name == "pragma") {
                handle_pragma(dir_args);
            } else if (dir_name == "line") {
                // #line directive - ignore for now
            } else if (dir_name == "embed") {
                // #embed "filename" - embed binary file contents
                output += handle_embed(dir_args) + "\n";
            } else {
                // Unknown directive - pass through
                output += line + "\n";
            }
        } else {
            // Regular line - expand macros
            if (!skipping) {
                output += expand_macros(line) + "\n";
            }
        }
    }
    
    // String literal concatenation: merge adjacent "..." "..." tokens
    // This is C translation phase 6, done here as a final preprocessor step
    {
        std::string result;
        size_t i = 0;
        while (i < output.size()) {
            if (output[i] != '"') {
                result += output[i];
                i++;
                continue;
            }
            std::string str;
            bool found_close = false;
            while (i < output.size() && output[i] != '\n') {
                if (output[i] == '\\' && i + 1 < output.size()) {
                    str += output[i]; str += output[i+1]; i += 2; continue;
                }
                str += output[i];
                i++;
                if (str.size() >= 2 && str.back() == '"') { found_close = true; break; }
            }
            size_t j = i;
            while (j < output.size() && (output[j] == ' ' || output[j] == '\t' || output[j] == '\n' || output[j] == '\r')) j++;
            if (found_close && j < output.size() && output[j] == '"') {
                str.pop_back();
                i = j + 1;
                while (i < output.size() && output[i] != '\n') {
                    if (output[i] == '\\' && i + 1 < output.size()) {
                        str += output[i]; str += output[i+1]; i += 2; continue;
                    }
                    str += output[i];
                    i++;
                    if (str.size() >= 1 && str.back() == '"') break;
                }
            } else {
                i = j;
            }
            result += str;
        }
        output = result;
    }

    return output;
}

std::string Preprocessor::expand_macros(const std::string& text) {
    std::string result;
    size_t i = 0;

    while (i < text.size()) {
        // Check for // or /* comments
        if (i < text.size() - 1 && text[i] == '/' && text[i+1] == '/') {
            result += text.substr(i);
            break;
        }
        if (i < text.size() - 1 && text[i] == '/' && text[i+1] == '*') {
            size_t end = text.find("*/", i + 2);
            if (end != std::string::npos) {
                result += text.substr(i, end - i + 2);
                i = end + 2;
            } else {
                result += text.substr(i);
                break;
            }
            continue;
        }
        
        // Check for string literal - skip macro expansion inside
        if (text[i] == '"') {
            size_t j = i + 1;
            while (j < text.size() && text[j] != '"') {
                if (text[j] == '\\') j++;
                j++;
            }
            result += text.substr(i, j - i + 1);
            i = j + 1;
            continue;
        }
        
        // Check for char literal
        if (text[i] == '\'') {
            size_t j = i + 1;
            while (j < text.size() && text[j] != '\'') {
                if (text[j] == '\\') j++;
                j++;
            }
            result += text.substr(i, j - i + 1);
            i = j + 1;
            continue;
        }
        
        // Check for identifier (potential macro)
        if (std::isalpha(text[i]) || text[i] == '_') {
            size_t j = i;
            while (j < text.size() && (std::isalnum(text[j]) || text[j] == '_')) j++;
            std::string ident = text.substr(i, j - i);
            
            // Check if it's a macro
            auto it = macros_.find(ident);
            if (it != macros_.end()) {
                const Macro& macro = it->second;
                
                if (macro.is_function_like) {
                    // Check for ( after macro name
                    size_t k = j;
                    while (k < text.size() && text[k] == ' ') k++;
                    
                    if (k < text.size() && text[k] == '(') {
                        // Parse arguments
                        k++; // skip (
                        std::vector<std::string> args;
                        std::string current_arg;
                        int depth = 1;
                        
                        while (k < text.size() && depth > 0) {
                            if (text[k] == '(') depth++;
                            else if (text[k] == ')') {
                                depth--;
                                if (depth == 0) {
                                    if (!current_arg.empty()) {
                                        // Trim whitespace
                                        size_t start = current_arg.find_first_not_of(" \t");
                                        size_t end = current_arg.find_last_not_of(" \t");
                                        if (start != std::string::npos) {
                                            args.push_back(current_arg.substr(start, end - start + 1));
                                        }
                                    }
                                    break;
                                }
                            } else if (text[k] == ',' && depth == 1) {
                                if (!current_arg.empty()) {
                                    size_t start = current_arg.find_first_not_of(" \t");
                                    size_t end = current_arg.find_last_not_of(" \t");
                                    if (start != std::string::npos) {
                                        args.push_back(current_arg.substr(start, end - start + 1));
                                    }
                                    current_arg.clear();
                                }
                            } else {
                                current_arg += text[k];
                            }
                            k++;
                        }
                        
                        result += expand_macro_call(macro, args);
                        i = k + 1;
                        continue;
                    }
                } else {
                    // Object-like macro
                    result += macro.body;
                    i = j;
                    continue;
                }
            }
            
            result += ident;
            i = j;
            continue;
        }
        
        // Regular character
        result += text[i];
        i++;
    }

    return result;
}

std::string Preprocessor::expand_macro_call(const Macro& macro, const std::vector<std::string>& args) {
    std::string result = macro.body;
    
    // First expand arguments (nested macro expansion)
    std::vector<std::string> expanded_args;
    for (const auto& arg : args) {
        expanded_args.push_back(expand_macros(arg));
    }
    
    // Handle variadic macros: __VA_ARGS__ gets the remaining args
    std::string va_args_str;
    if (macro.is_variadic && expanded_args.size() > macro.params.size()) {
        for (size_t i = macro.params.size(); i < expanded_args.size(); i++) {
            if (i > macro.params.size()) va_args_str += ", ";
            va_args_str += expanded_args[i];
        }
    }
    
    // Handle # stringification in body
    for (size_t i = 0; i < macro.params.size() && i < expanded_args.size(); i++) {
        std::string hash_pattern = "#" + macro.params[i];
        size_t pos = 0;
        while ((pos = result.find(hash_pattern, pos)) != std::string::npos) {
            // Skip if this # is part of ## (preceded by another #)
            if (pos > 0 && result[pos - 1] == '#') {
                pos += hash_pattern.size();
                continue;
            }
            result.replace(pos, hash_pattern.size(), "\"" + expanded_args[i] + "\"");
            pos += expanded_args[i].size() + 2;
        }
    }
    
    // Handle ## token pasting in body
    for (size_t i = 0; i < macro.params.size() && i < expanded_args.size(); i++) {
        std::string paste_pattern = macro.params[i] + "##";
        size_t pos = 0;
        while ((pos = result.find(paste_pattern, pos)) != std::string::npos) {
            size_t after = pos + paste_pattern.size();
            while (after < result.size() && result[after] == ' ') after++;
            size_t end = after;
            while (end < result.size() && (std::isalnum(result[end]) || result[end] == '_')) end++;
            std::string next_token = result.substr(after, end - after);
            result.replace(pos, end - pos, expanded_args[i] + next_token);
            pos += expanded_args[i].size() + next_token.size();
        }
    }
    
    // Replace __VA_ARGS__
    size_t va_pos = result.find("__VA_ARGS__");
    while (va_pos != std::string::npos) {
        result.replace(va_pos, 11, va_args_str);
        va_pos = result.find("__VA_ARGS__", va_pos + va_args_str.size());
    }
    
    // Replace regular parameters with expanded arguments
    for (size_t i = 0; i < macro.params.size() && i < expanded_args.size(); i++) {
        size_t pos = 0;
        while ((pos = result.find(macro.params[i], pos)) != std::string::npos) {
            result.replace(pos, macro.params[i].size(), expanded_args[i]);
            pos += expanded_args[i].size();
        }
    }
    
    return result;
}

void Preprocessor::handle_include(const std::string& args) {
    // For now, just track that we saw an include
    // Full implementation would read the file and process it
}

void Preprocessor::handle_define(const std::string& args) {
    size_t space = args.find_first_of(" \t(");
    std::string name = args.substr(0, space);
    
    if (space == std::string::npos) {
        // Object-like macro with empty body
        macros_[name] = Macro(name, "");
        return;
    }
    
    if (args[space] == '(') {
        // Function-like macro
        size_t close = args.find(')', space);
        if (close == std::string::npos) {
            error_message_ = "Unterminated macro parameter list";
            error_line_ = 0;
            return;
        }
        
        std::string params_str = args.substr(space + 1, close - space - 1);
        std::vector<std::string> params = split_args(params_str);
        bool is_variadic = false;
        
        // Check for variadic ... at end of params
        if (!params.empty() && params.back() == "...") {
            is_variadic = true;
            params.pop_back();
        }
        
        std::string body = args.substr(close + 1);
        size_t bstart = body.find_first_not_of(" \t");
        if (bstart != std::string::npos) body = body.substr(bstart);
        
        Macro macro(name, body, true, is_variadic);
        macro.params = params;
        macros_[name] = macro;
    } else {
        // Object-like macro
        std::string body = args.substr(space + 1);
        size_t start = body.find_first_not_of(" \t");
        if (start != std::string::npos) body = body.substr(start);
        macros_[name] = Macro(name, body);
    }
}

void Preprocessor::handle_undef(const std::string& args) {
    std::string name = args;
    size_t end = name.find_first_of(" \t");
    if (end != std::string::npos) name = name.substr(0, end);
    macros_.erase(name);
}

void Preprocessor::handle_ifdef(const std::string& args) {
    std::string name = args;
    size_t end = name.find_first_of(" \t");
    if (end != std::string::npos) name = name.substr(0, end);
    
    bool defined = macros_.find(name) != macros_.end();
    condition_stack_.push_back(defined);
}

void Preprocessor::handle_ifndef(const std::string& args) {
    std::string name = args;
    size_t end = name.find_first_of(" \t");
    if (end != std::string::npos) name = name.substr(0, end);
    
    bool not_defined = macros_.find(name) == macros_.end();
    condition_stack_.push_back(not_defined);
}

void Preprocessor::handle_if(const std::string& args) {
    // Simplified: just check if the condition is "0" or "1"
    bool result = evaluate_condition(args);
    condition_stack_.push_back(result);
}

void Preprocessor::handle_else() {
    if (!condition_stack_.empty()) {
        bool val = condition_stack_.back();
        condition_stack_.pop_back();
        condition_stack_.push_back(!val);
    }
}

void Preprocessor::handle_endif() {
    if (!condition_stack_.empty()) {
        condition_stack_.pop_back();
    }
}

void Preprocessor::handle_error(const std::string& args) {
    error_message_ = "#error: " + args;
    error_line_ = 0;
}

void Preprocessor::handle_pragma(const std::string& args) {
    // Ignore pragmas for now
}

bool Preprocessor::evaluate_condition(const std::string& condition) {
    std::string trimmed = condition;
    size_t start = trimmed.find_first_not_of(" \t");
    if (start != std::string::npos) trimmed = trimmed.substr(start);
    size_t end = trimmed.find_last_not_of(" \t");
    if (end != std::string::npos) trimmed = trimmed.substr(0, end + 1);
    
    // Check for defined() operator
    if (trimmed.find("defined(") == 0) {
        std::string name = trimmed.substr(8);
        size_t close = name.find(')');
        if (close != std::string::npos) name = name.substr(0, close);
        return macros_.find(name) != macros_.end();
    }
    
    // Check for defined operator without parens
    if (trimmed.find("defined ") == 0) {
        std::string name = trimmed.substr(8);
        return macros_.find(name) != macros_.end();
    }
    
    // Simple integer comparison
    if (trimmed == "0" || trimmed == "false") return false;
    if (trimmed == "1" || trimmed == "true") return true;
    
    // Check if it's a macro name
    if (macros_.find(trimmed) != macros_.end()) {
        return true;
    }
    
    // Default: treat as true if non-zero
    return !trimmed.empty() && trimmed != "0";
}

std::string Preprocessor::read_file(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        error_message_ = "Cannot open file: " + filename;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

std::vector<std::string> Preprocessor::split_args(const std::string& args) {
    std::vector<std::string> result;
    std::string current;
    int depth = 0;
    
    for (char c : args) {
        if (c == '(') depth++;
        else if (c == ')') depth--;
        
        if (c == ',' && depth == 0) {
            if (!current.empty()) {
                result.push_back(current);
                current.clear();
            }
        } else {
            current += c;
        }
    }
    
    if (!current.empty()) {
        result.push_back(current);
    }
    
    // Trim whitespace from each parameter
    for (auto& s : result) {
        size_t start = s.find_first_not_of(" \t");
        size_t end = s.find_last_not_of(" \t");
        if (start != std::string::npos) {
            s = s.substr(start, end - start + 1);
        }
    }
    
    return result;
}

void Preprocessor::define_macro(const std::string& name, const std::string& body) {
    macros_[name] = Macro(name, body);
}

bool Preprocessor::is_defined(const std::string& name) const {
    return macros_.find(name) != macros_.end();
}

std::string Preprocessor::handle_embed(const std::string& args) {
    // Parse filename from args: "filename" or <filename>
    std::string filename;
    size_t start = args.find_first_not_of(" \t");
    if (start == std::string::npos) {
        error_message_ = "#embed: missing filename";
        return "";
    }
    
    if (args[start] == '"') {
        size_t end = args.find('"', start + 1);
        if (end == std::string::npos) {
            error_message_ = "#embed: unterminated filename";
            return "";
        }
        filename = args.substr(start + 1, end - start - 1);
    } else if (args[start] == '<') {
        size_t end = args.find('>', start + 1);
        if (end == std::string::npos) {
            error_message_ = "#embed: unterminated filename";
            return "";
        }
        filename = args.substr(start + 1, end - start - 1);
    } else {
        filename = args.substr(start);
        // Trim trailing whitespace
        size_t end = filename.find_last_not_of(" \t");
        if (end != std::string::npos) filename = filename.substr(0, end + 1);
    }
    
    // Read binary file
    std::string content = read_file(filename);
    if (content.empty() && !error_message_.empty()) {
        return "";
    }
    
    // Convert to comma-separated integer values
    std::string result;
    for (size_t i = 0; i < content.size(); i++) {
        if (i > 0) result += ", ";
        result += std::to_string(static_cast<unsigned char>(content[i]));
    }
    
    return result;
}

} // namespace simplecc
