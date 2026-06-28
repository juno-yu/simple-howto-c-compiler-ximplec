#include "ast.h"

namespace simplecc {

// Accept method implementations
void ProgramNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void ParamNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void FunctionDeclNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void VarDeclNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void StructFieldNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void StructDeclNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void EnumDeclNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void EnumValueNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void TypedefDeclNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void SwitchStmtNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void CaseLabelNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void DefaultLabelNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void GotoStmtNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void LabelStmtNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void BlockNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void ReturnStmtNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void ExprStmtNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void IfStmtNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void WhileStmtNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void DoWhileStmtNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void ForStmtNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void BreakStmtNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void ContinueStmtNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void BinaryExprNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void UnaryExprNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void AssignExprNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void CompoundAssignExprNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void TernaryExprNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void CommaExprNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void SizeofExprNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void CastExprNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void CallExprNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void IndexExprNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void MemberExprNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void DerefExprNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void AddressOfExprNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void StmtExprNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void IntegerLiteralNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void FloatLiteralNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void StringLiteralNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void CharLiteralNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void IdentifierExprNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }

const char* node_type_name(NodeType type) {
    switch (type) {
        case NodeType::PROGRAM: return "Program";
        case NodeType::FUNCTION_DECL: return "FunctionDecl";
        case NodeType::VAR_DECL: return "VarDecl";
        case NodeType::STRUCT_FIELD: return "StructField";
        case NodeType::STRUCT_DECL: return "StructDecl";
        case NodeType::ENUM_DECL: return "EnumDecl";
        case NodeType::ENUM_VALUE: return "EnumValue";
        case NodeType::TYPEDEF_DECL: return "TypedefDecl";
        case NodeType::SWITCH_STMT: return "SwitchStmt";
        case NodeType::CASE_LABEL: return "CaseLabel";
        case NodeType::DEFAULT_LABEL: return "DefaultLabel";
        case NodeType::GOTO_STMT: return "GotoStmt";
        case NodeType::LABEL_STMT: return "LabelStmt";
        case NodeType::PARAM: return "Param";
        case NodeType::BLOCK: return "Block";
        case NodeType::RETURN_STMT: return "ReturnStmt";
        case NodeType::EXPR_STMT: return "ExprStmt";
        case NodeType::IF_STMT: return "IfStmt";
        case NodeType::WHILE_STMT: return "WhileStmt";
        case NodeType::DO_WHILE_STMT: return "DoWhileStmt";
        case NodeType::FOR_STMT: return "ForStmt";
        case NodeType::BREAK_STMT: return "BreakStmt";
        case NodeType::CONTINUE_STMT: return "ContinueStmt";
        case NodeType::BINARY_EXPR: return "BinaryExpr";
        case NodeType::UNARY_EXPR: return "UnaryExpr";
        case NodeType::ASSIGN_EXPR: return "AssignExpr";
        case NodeType::COMPOUND_ASSIGN_EXPR: return "CompoundAssignExpr";
        case NodeType::TERNARY_EXPR: return "TernaryExpr";
        case NodeType::COMMA_EXPR: return "CommaExpr";
        case NodeType::SIZEOF_EXPR: return "SizeofExpr";
        case NodeType::CAST_EXPR: return "CastExpr";
        case NodeType::CALL_EXPR: return "CallExpr";
        case NodeType::INDEX_EXPR: return "IndexExpr";
        case NodeType::MEMBER_EXPR: return "MemberExpr";
        case NodeType::DEREF_EXPR: return "DerefExpr";
        case NodeType::ADDRESS_OF_EXPR: return "AddressOfExpr";
        case NodeType::INTEGER_LITERAL: return "IntegerLiteral";
        case NodeType::FLOAT_LITERAL: return "FloatLiteral";
        case NodeType::STRING_LITERAL: return "StringLiteral";
        case NodeType::CHAR_LITERAL: return "CharLiteral";
        case NodeType::IDENTIFIER_EXPR: return "IdentifierExpr";
    }
    return "Unknown";
}

const char* op_kind_name(OpKind op) {
    switch (op) {
        case OpKind::ADD: return "+";
        case OpKind::SUB: return "-";
        case OpKind::MUL: return "*";
        case OpKind::DIV: return "/";
        case OpKind::MOD: return "%";
        case OpKind::EQ: return "==";
        case OpKind::NE: return "!=";
        case OpKind::LT: return "<";
        case OpKind::GT: return ">";
        case OpKind::LE: return "<=";
        case OpKind::GE: return ">=";
        case OpKind::AND: return "&&";
        case OpKind::OR: return "||";
        case OpKind::NOT: return "!";
        case OpKind::BIT_AND: return "&";
        case OpKind::BIT_OR: return "|";
        case OpKind::BIT_XOR: return "^";
        case OpKind::BIT_NOT: return "~";
        case OpKind::LSHIFT: return "<<";
        case OpKind::RSHIFT: return ">>";
        case OpKind::UMINUS: return "-";
        case OpKind::UPLUS: return "+";
        case OpKind::DEREF: return "*";
        case OpKind::ADDRESS_OF: return "&";
        case OpKind::PRE_INC: return "++";
        case OpKind::PRE_DEC: return "--";
        case OpKind::POST_INC: return "++";
        case OpKind::POST_DEC: return "--";
    }
    return "Unknown";
}

} // namespace simplecc
