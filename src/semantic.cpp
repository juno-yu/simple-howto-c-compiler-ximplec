#include "semantic.h"
#include <algorithm>

namespace simplecc {

SemanticAnalyzer::SemanticAnalyzer() : error_line_(0), scope_level_(0) {
    // Initialize built-in types
    types_["int"] = TypeInfo();
    types_["int"].name = "int";
    types_["int"].size = 4;
    
    types_["char"] = TypeInfo();
    types_["char"].name = "char";
    types_["char"].size = 1;
    
    types_["void"] = TypeInfo();
    types_["void"].name = "void";
    types_["void"].size = 0;
    
    types_["bool"] = TypeInfo();
    types_["bool"].name = "bool";
    types_["bool"].size = 1;
    
    types_["long"] = TypeInfo();
    types_["long"].name = "long";
    types_["long"].size = 8;
    
    types_["short"] = TypeInfo();
    types_["short"].name = "short";
    types_["short"].size = 2;
    
    types_["float"] = TypeInfo();
    types_["float"].name = "float";
    types_["float"].size = 4;
    
    types_["double"] = TypeInfo();
    types_["double"].name = "double";
    types_["double"].size = 8;
    
    types_["unsigned int"] = TypeInfo();
    types_["unsigned int"].name = "unsigned int";
    types_["unsigned int"].size = 4;
    
    types_["size_t"] = TypeInfo();
    types_["size_t"].name = "unsigned long";
    types_["size_t"].size = 8;
}

bool SemanticAnalyzer::analyze(ProgramNode& program) {
    push_scope();
    
    for (auto& decl : program.declarations) {
        dispatch(decl.get());
    }
    
    pop_scope();
    return !has_error();
}

void SemanticAnalyzer::dispatch(ASTNode* node) {
    if (!node) return;
    
    switch (node->type) {
        case NodeType::PROGRAM:
            visit(static_cast<ProgramNode&>(*node));
            break;
        case NodeType::FUNCTION_DECL:
            visit(static_cast<FunctionDeclNode&>(*node));
            break;
        case NodeType::VAR_DECL:
            visit(static_cast<VarDeclNode&>(*node));
            break;
        case NodeType::BLOCK:
            visit(static_cast<BlockNode&>(*node));
            break;
        case NodeType::RETURN_STMT:
            visit(static_cast<ReturnStmtNode&>(*node));
            break;
        case NodeType::IF_STMT:
            visit(static_cast<IfStmtNode&>(*node));
            break;
        case NodeType::WHILE_STMT:
            visit(static_cast<WhileStmtNode&>(*node));
            break;
        case NodeType::FOR_STMT:
            visit(static_cast<ForStmtNode&>(*node));
            break;
        case NodeType::EXPR_STMT:
            visit(static_cast<ExprStmtNode&>(*node));
            break;
        case NodeType::BINARY_EXPR:
            visit(static_cast<BinaryExprNode&>(*node));
            break;
        case NodeType::UNARY_EXPR:
            visit(static_cast<UnaryExprNode&>(*node));
            break;
        case NodeType::ASSIGN_EXPR:
            visit(static_cast<AssignExprNode&>(*node));
            break;
        case NodeType::CALL_EXPR:
            visit(static_cast<CallExprNode&>(*node));
            break;
        case NodeType::INTEGER_LITERAL:
            visit(static_cast<IntegerLiteralNode&>(*node));
            break;
        case NodeType::STRING_LITERAL:
            visit(static_cast<StringLiteralNode&>(*node));
            break;
        case NodeType::IDENTIFIER_EXPR:
            visit(static_cast<IdentifierExprNode&>(*node));
            break;
        default:
            // Ignore other node types
            break;
    }
}

void SemanticAnalyzer::visit(ProgramNode& node) {
    for (auto& decl : node.declarations) {
        if (decl->type == NodeType::FUNCTION_DECL) {
            visit(static_cast<FunctionDeclNode&>(*decl));
        } else if (decl->type == NodeType::VAR_DECL) {
            visit(static_cast<VarDeclNode&>(*decl));
        }
    }
}

void SemanticAnalyzer::visit(FunctionDeclNode& node) {
    // Check if function is already declared
    Symbol* existing = lookup_symbol(node.name);
    if (existing && existing->kind == SymbolKind::FUNCTION) {
        // Allow redeclaration with same signature
    } else {
        Symbol sym;
        sym.name = node.name;
        sym.type = node.return_type;
        sym.kind = SymbolKind::FUNCTION;
        sym.scope_level = scope_level_;
        sym.line = node.line;
        sym.column = node.column;
        
        if (!declare_symbol(sym)) {
            error("Redefinition of '" + node.name + "'", node.line, node.column);
            return;
        }
    }
    
    // Process function body if present
    if (node.body) {
        std::string prev_function = current_function_;
        std::string prev_return = current_return_type_;
        current_function_ = node.name;
        current_return_type_ = node.return_type;
        
        push_scope();
        
        // Add parameters to scope
        for (auto& param : node.params) {
            if (param->type == NodeType::PARAM) {
                auto* p = static_cast<ParamNode*>(param.get());
                Symbol sym;
                sym.name = p->name;
                sym.type = p->type_name;
                sym.kind = SymbolKind::PARAMETER;
                sym.scope_level = scope_level_;
                sym.line = p->line;
                sym.column = p->column;
                declare_symbol(sym);
            }
        }
        
        dispatch(node.body.get());
        
        pop_scope();
        current_function_ = prev_function;
        current_return_type_ = prev_return;
    }
}

void SemanticAnalyzer::visit(VarDeclNode& node) {
    // Check if variable is already declared in current scope
    Symbol* existing = lookup_symbol(node.name);
    if (existing && existing->scope_level == scope_level_) {
        error("Redefinition of '" + node.name + "'", node.line, node.column);
        return;
    }
    
    Symbol sym;
    sym.name = node.name;
    sym.type = node.type_name;
    sym.kind = SymbolKind::VARIABLE;
    sym.scope_level = scope_level_;
    sym.is_const = node.type_name.find("const") != std::string::npos;
    sym.is_static = node.type_name.find("static") != std::string::npos;
    sym.is_extern = node.type_name.find("extern") != std::string::npos;
    sym.line = node.line;
    sym.column = node.column;
    
    if (!declare_symbol(sym)) {
        error("Cannot declare variable '" + node.name + "'", node.line, node.column);
    }
}

void SemanticAnalyzer::visit(BlockNode& node) {
    push_scope();
    for (auto& stmt : node.statements) {
        dispatch(stmt.get());
    }
    pop_scope();
}

void SemanticAnalyzer::visit(ReturnStmtNode& node) {
    if (current_function_.empty()) {
        error("Return statement outside function", node.line, node.column);
        return;
    }
    
    if (node.value) {
        std::string expr_type = get_expr_type(node.value.get());
        if (!types_compatible(current_return_type_, expr_type)) {
            error("Return type mismatch in function '" + current_function_ + "'", node.line, node.column);
        }
    } else if (current_return_type_ != "void") {
        error("Non-void function '" + current_function_ + "' returns no value", node.line, node.column);
    }
}

void SemanticAnalyzer::visit(IfStmtNode& node) {
    if (node.condition) dispatch(node.condition.get());
    if (node.then_branch) dispatch(node.then_branch.get());
    if (node.else_branch) dispatch(node.else_branch.get());
}

void SemanticAnalyzer::visit(WhileStmtNode& node) {
    if (node.condition) dispatch(node.condition.get());
    if (node.body) dispatch(node.body.get());
}

void SemanticAnalyzer::visit(ForStmtNode& node) {
    push_scope();
    if (node.init) dispatch(node.init.get());
    if (node.condition) dispatch(node.condition.get());
    if (node.update) dispatch(node.update.get());
    if (node.body) dispatch(node.body.get());
    pop_scope();
}

void SemanticAnalyzer::visit(ExprStmtNode& node) {
    if (node.expr) dispatch(node.expr.get());
}

void SemanticAnalyzer::visit(BinaryExprNode& node) {
    if (node.left) dispatch(node.left.get());
    if (node.right) dispatch(node.right.get());
}

void SemanticAnalyzer::visit(UnaryExprNode& node) {
    if (node.operand) dispatch(node.operand.get());
}

void SemanticAnalyzer::visit(AssignExprNode& node) {
    if (node.value) dispatch(node.value.get());
    if (node.target) dispatch(node.target.get());
}

void SemanticAnalyzer::visit(CallExprNode& node) {
    Symbol* sym = lookup_symbol(node.function_name);
    if (!sym) {
        // Allow undeclared functions (will be caught at link time)
    } else if (sym->kind != SymbolKind::FUNCTION) {
        error("'" + node.function_name + "' is not a function", node.line, node.column);
    }
    // Don't dispatch into arguments - just check the function exists
}

void SemanticAnalyzer::visit(IntegerLiteralNode& node) {
    // Nothing to do
}

void SemanticAnalyzer::visit(StringLiteralNode& node) {
    // Nothing to do
}

void SemanticAnalyzer::visit(IdentifierExprNode& node) {
    Symbol* sym = lookup_symbol(node.name);
    if (!sym) {
        // Allow undeclared identifiers (will be caught at link time)
    } else {
        sym->is_used = true;
    }
}

// Scope management
void SemanticAnalyzer::push_scope() {
    scope_level_++;
    scope_stack_.push_back(std::map<std::string, Symbol>());
}

void SemanticAnalyzer::pop_scope() {
    if (!scope_stack_.empty()) {
        scope_stack_.pop_back();
    }
    scope_level_--;
}

bool SemanticAnalyzer::declare_symbol(const Symbol& sym) {
    if (scope_stack_.empty()) return false;
    
    auto& current_scope = scope_stack_.back();
    if (current_scope.find(sym.name) != current_scope.end()) {
        return false; // Already declared in this scope
    }
    
    current_scope[sym.name] = sym;
    symbols_[sym.name] = sym;
    return true;
}

Symbol* SemanticAnalyzer::lookup_symbol(const std::string& name) {
    // Search from innermost to outermost scope
    for (int i = scope_stack_.size() - 1; i >= 0; i--) {
        auto it = scope_stack_[i].find(name);
        if (it != scope_stack_[i].end()) {
            return &(it->second);
        }
    }
    
    // Check global symbols
    auto it = symbols_.find(name);
    if (it != symbols_.end()) {
        return &(it->second);
    }
    
    return nullptr;
}

std::string SemanticAnalyzer::get_expr_type(ASTNode* node) {
    if (!node) return "void";
    
    switch (node->type) {
        case NodeType::INTEGER_LITERAL:
            return "int";
        case NodeType::CHAR_LITERAL:
            return "char";
        case NodeType::STRING_LITERAL:
            return "char*";
        case NodeType::FLOAT_LITERAL:
            return "double";
        case NodeType::IDENTIFIER_EXPR: {
            auto* id = static_cast<IdentifierExprNode*>(node);
            Symbol* sym = lookup_symbol(id->name);
            if (sym) return sym->type;
            return "int"; // Default
        }
        case NodeType::BINARY_EXPR: {
            auto* bin = static_cast<BinaryExprNode*>(node);
            return get_expr_type(bin->left.get());
        }
        case NodeType::UNARY_EXPR: {
            auto* un = static_cast<UnaryExprNode*>(node);
            if (un->op == OpKind::ADDRESS_OF) {
                return get_expr_type(un->operand.get()) + "*";
            }
            if (un->op == OpKind::DEREF) {
                std::string t = get_expr_type(un->operand.get());
                if (t.size() > 0 && t.back() == '*') {
                    t.pop_back();
                }
                return t;
            }
            return get_expr_type(un->operand.get());
        }
        case NodeType::CALL_EXPR: {
            auto* call = static_cast<CallExprNode*>(node);
            Symbol* sym = lookup_symbol(call->function_name);
            if (sym) return sym->type;
            return "int";
        }
        default:
            return "int";
    }
}

bool SemanticAnalyzer::types_compatible(const std::string& t1, const std::string& t2) {
    // Simple compatibility check
    if (t1 == t2) return true;
    
    // Allow implicit conversions between int types
    std::vector<std::string> int_types = {"int", "char", "long", "short", "bool",
                                           "unsigned int", "unsigned long", "unsigned short"};
    bool t1_is_int = std::find(int_types.begin(), int_types.end(), t1) != int_types.end();
    bool t2_is_int = std::find(int_types.begin(), int_types.end(), t2) != int_types.end();
    
    if (t1_is_int && t2_is_int) return true;
    
    // Allow pointer conversions
    if (t1.size() > 0 && t1.back() == '*' && t2.size() > 0 && t2.back() == '*') return true;
    if (t1.size() > 0 && t1.back() == '*' && t2 == "int") return true; // int to pointer
    if (t1 == "int" && t2.size() > 0 && t2.back() == '*') return true; // pointer to int
    
    return false;
}

int SemanticAnalyzer::get_type_size(const std::string& type) {
    auto it = types_.find(type);
    if (it != types_.end()) {
        return it->second.size;
    }
    if (type.size() > 0 && type.back() == '*') return 8; // pointer
    return 4; // default
}

void SemanticAnalyzer::error(const std::string& msg, int line, int col) {
    if (error_message_.empty()) {
        error_message_ = "Semantic error: " + msg + " at line " + std::to_string(line) +
                        ", column " + std::to_string(col);
        error_line_ = line;
    }
}

} // namespace simplecc
