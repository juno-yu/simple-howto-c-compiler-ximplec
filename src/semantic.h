#pragma once

#include "ast.h"
#include <string>
#include <vector>
#include <map>
#include <memory>

namespace simplecc {

enum class SymbolKind {
    VARIABLE,
    FUNCTION,
    PARAMETER,
    TYPE,
    STRUCT,
    ENUM,
    UNION
};

struct Symbol {
    std::string name;
    std::string type;
    SymbolKind kind;
    int scope_level;
    bool is_const;
    bool is_static;
    bool is_extern;
    bool is_used;
    int line;
    int column;
    
    Symbol() : kind(SymbolKind::VARIABLE), scope_level(0), is_const(false),
               is_static(false), is_extern(false), is_used(false), line(0), column(0) {}
};

struct TypeInfo {
    std::string name;
    int size;
    bool is_pointer;
    bool is_array;
    int array_size;
    bool is_function;
    std::string return_type;
    std::vector<std::string> param_types;
    
    TypeInfo() : size(4), is_pointer(false), is_array(false), array_size(0),
                 is_function(false) {}
};

class SemanticAnalyzer {
public:
    SemanticAnalyzer();
    ~SemanticAnalyzer() = default;
    
    // Reset state for separate compilation of a new file
    void reset();
    
    bool analyze(ProgramNode& program);
    
    bool has_error() const { return !error_message_.empty(); }
    const std::string& error_message() const { return error_message_; }
    int error_line() const { return error_line_; }
    
    // Get symbol table for external use
    const std::map<std::string, Symbol>& get_symbols() const { return symbols_; }
    
private:
    std::string error_message_;
    int error_line_;
    int scope_level_;
    
    // Symbol table
    std::map<std::string, Symbol> symbols_;
    
    // Type information
    std::map<std::string, TypeInfo> types_;
    
    // Scope tracking
    std::vector<std::map<std::string, Symbol>> scope_stack_;
    
    // Current function context
    std::string current_function_;
    std::string current_return_type_;
    
    // Visitor methods
    void dispatch(ASTNode* node);
    void visit(ProgramNode& node);
    void visit(FunctionDeclNode& node);
    void visit(VarDeclNode& node);
    void visit(BlockNode& node);
    void visit(ReturnStmtNode& node);
    void visit(IfStmtNode& node);
    void visit(WhileStmtNode& node);
    void visit(ForStmtNode& node);
    void visit(ExprStmtNode& node);
    void visit(BinaryExprNode& node);
    void visit(UnaryExprNode& node);
    void visit(AssignExprNode& node);
    void visit(CallExprNode& node);
    void visit(IntegerLiteralNode& node);
    void visit(StringLiteralNode& node);
    void visit(IdentifierExprNode& node);
    
    // Scope management
    void push_scope();
    void pop_scope();
    bool declare_symbol(const Symbol& sym);
    Symbol* lookup_symbol(const std::string& name);
    
    // Type checking
    std::string get_expr_type(ASTNode* node);
    bool types_compatible(const std::string& t1, const std::string& t2);
    int get_type_size(const std::string& type);
    
    // Error reporting
    void error(const std::string& msg, int line, int col);
};

} // namespace simplecc
