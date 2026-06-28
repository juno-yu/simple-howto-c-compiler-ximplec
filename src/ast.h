#pragma once

#include "token.h"
#include <string>
#include <vector>
#include <memory>

namespace simplecc {

enum class NodeType {
    // Program
    PROGRAM,
    
    // Declarations
    FUNCTION_DECL,
    VAR_DECL,
    PARAM,
    STRUCT_DECL,
    STRUCT_FIELD,
    ENUM_DECL,
    ENUM_VALUE,
    TYPEDEF_DECL,
    SWITCH_STMT,
    CASE_LABEL,
    DEFAULT_LABEL,
    GOTO_STMT,
    LABEL_STMT,
    
    // Statements
    BLOCK,
    RETURN_STMT,
    EXPR_STMT,
    IF_STMT,
    WHILE_STMT,
    DO_WHILE_STMT,
    FOR_STMT,
    BREAK_STMT,
    CONTINUE_STMT,
    
    // Expressions
    BINARY_EXPR,
    UNARY_EXPR,
    ASSIGN_EXPR,
    COMPOUND_ASSIGN_EXPR,
    TERNARY_EXPR,
    COMMA_EXPR,
    SIZEOF_EXPR,
    CAST_EXPR,
    CALL_EXPR,
    INDEX_EXPR,
    MEMBER_EXPR,
    DEREF_EXPR,
    ADDRESS_OF_EXPR,
    STMT_EXPR,
    
    // Literals
    INTEGER_LITERAL,
    FLOAT_LITERAL,
    STRING_LITERAL,
    CHAR_LITERAL,
    IDENTIFIER_EXPR,
};

enum class OpKind {
    // Arithmetic
    ADD, SUB, MUL, DIV, MOD,
    
    // Comparison
    EQ, NE, LT, GT, LE, GE,
    
    // Logical
    AND, OR, NOT,
    
    // Bitwise
    BIT_AND, BIT_OR, BIT_XOR, BIT_NOT,
    LSHIFT, RSHIFT,
    
    // Unary
    UMINUS, UPLUS, DEREF, ADDRESS_OF,
    PRE_INC, PRE_DEC, POST_INC, POST_DEC,
};

// Forward declarations
struct ProgramNode;
struct ParamNode;
struct FunctionDeclNode;
struct VarDeclNode;
struct StructFieldNode;
struct StructDeclNode;
struct EnumDeclNode;
struct EnumValueNode;
struct TypedefDeclNode;
struct SwitchStmtNode;
struct CaseLabelNode;
struct DefaultLabelNode;
struct GotoStmtNode;
struct LabelStmtNode;
struct BlockNode;
struct ReturnStmtNode;
struct ExprStmtNode;
struct IfStmtNode;
struct WhileStmtNode;
struct DoWhileStmtNode;
struct ForStmtNode;
struct BreakStmtNode;
struct ContinueStmtNode;
struct BinaryExprNode;
struct UnaryExprNode;
struct AssignExprNode;
struct CompoundAssignExprNode;
struct TernaryExprNode;
struct CommaExprNode;
struct SizeofExprNode;
struct CastExprNode;
struct CallExprNode;
struct IndexExprNode;
struct MemberExprNode;
struct DerefExprNode;
struct AddressOfExprNode;
struct StmtExprNode;
struct IntegerLiteralNode;
struct FloatLiteralNode;
struct StringLiteralNode;
struct CharLiteralNode;
struct IdentifierExprNode;

// Visitor pattern
class ASTVisitor {
public:
    virtual ~ASTVisitor() = default;
    
    virtual void visit(ProgramNode& node) = 0;
    virtual void visit(FunctionDeclNode& node) = 0;
    virtual void visit(VarDeclNode& node) = 0;
    virtual void visit(StructFieldNode& node) = 0;
    virtual void visit(StructDeclNode& node) = 0;
    virtual void visit(EnumDeclNode& node) = 0;
    virtual void visit(EnumValueNode& node) = 0;
    virtual void visit(TypedefDeclNode& node) = 0;
    virtual void visit(SwitchStmtNode& node) = 0;
    virtual void visit(CaseLabelNode& node) = 0;
    virtual void visit(DefaultLabelNode& node) = 0;
    virtual void visit(GotoStmtNode& node) = 0;
    virtual void visit(LabelStmtNode& node) = 0;
    virtual void visit(ParamNode& node) = 0;
    virtual void visit(BlockNode& node) = 0;
    virtual void visit(ReturnStmtNode& node) = 0;
    virtual void visit(ExprStmtNode& node) = 0;
    virtual void visit(IfStmtNode& node) = 0;
    virtual void visit(WhileStmtNode& node) = 0;
    virtual void visit(DoWhileStmtNode& node) = 0;
    virtual void visit(ForStmtNode& node) = 0;
    virtual void visit(BreakStmtNode& node) = 0;
    virtual void visit(ContinueStmtNode& node) = 0;
    virtual void visit(BinaryExprNode& node) = 0;
    virtual void visit(UnaryExprNode& node) = 0;
    virtual void visit(AssignExprNode& node) = 0;
    virtual void visit(CompoundAssignExprNode& node) = 0;
    virtual void visit(TernaryExprNode& node) = 0;
    virtual void visit(CommaExprNode& node) = 0;
    virtual void visit(SizeofExprNode& node) = 0;
    virtual void visit(CastExprNode& node) = 0;
    virtual void visit(CallExprNode& node) = 0;
    virtual void visit(IndexExprNode& node) = 0;
    virtual void visit(MemberExprNode& node) = 0;
    virtual void visit(DerefExprNode& node) = 0;
    virtual void visit(AddressOfExprNode& node) = 0;
    virtual void visit(StmtExprNode& node) = 0;
    virtual void visit(IntegerLiteralNode& node) = 0;
    virtual void visit(FloatLiteralNode& node) = 0;
    virtual void visit(StringLiteralNode& node) = 0;
    virtual void visit(CharLiteralNode& node) = 0;
    virtual void visit(IdentifierExprNode& node) = 0;
};

struct ASTNode {
    NodeType type;
    int line;
    int column;
    
    ASTNode(NodeType t, int l, int c) : type(t), line(l), column(c) {}
    virtual ~ASTNode() = default;
};

using ASTPtr = std::unique_ptr<ASTNode>;

// Program
struct ProgramNode : ASTNode {
    std::vector<ASTPtr> declarations;
    
    ProgramNode() : ASTNode(NodeType::PROGRAM, 0, 0) {}
    void accept(ASTVisitor& visitor);
};

// Declarations
struct ParamNode : ASTNode {
    std::string type_name;
    std::string name;
    
    ParamNode(const std::string& type, const std::string& n, int l, int c)
        : ASTNode(NodeType::PARAM, l, c), type_name(type), name(n) {}
    void accept(ASTVisitor& visitor);
};

struct FunctionDeclNode : ASTNode {
    std::string return_type;
    std::string name;
    std::vector<ASTPtr> params;
    ASTPtr body;
    
    FunctionDeclNode(const std::string& ret, const std::string& n, int l, int c)
        : ASTNode(NodeType::FUNCTION_DECL, l, c), return_type(ret), name(n) {}
    void accept(ASTVisitor& visitor);
};

struct VarDeclNode : ASTNode {
    std::string type_name;
    std::string name;
    ASTPtr initializer;
    int array_size; // 0 = not an array, >0 = array size
    
    VarDeclNode(const std::string& type, const std::string& n, int l, int c)
        : ASTNode(NodeType::VAR_DECL, l, c), type_name(type), name(n), array_size(0) {}
    void accept(ASTVisitor& visitor);
};

struct StructFieldNode : ASTNode {
    std::string type_name;
    std::string name;
    
    StructFieldNode(const std::string& type, const std::string& n, int l, int c)
        : ASTNode(NodeType::STRUCT_FIELD, l, c), type_name(type), name(n) {}
    void accept(ASTVisitor& visitor);
};

struct StructDeclNode : ASTNode {
    std::string name;
    std::vector<ASTPtr> fields;
    
    StructDeclNode(const std::string& n, int l, int c)
        : ASTNode(NodeType::STRUCT_DECL, l, c), name(n) {}
    void accept(ASTVisitor& visitor);
};

struct EnumDeclNode : ASTNode {
    std::string name;
    std::vector<ASTPtr> values;
    
    EnumDeclNode(const std::string& n, int l, int c)
        : ASTNode(NodeType::ENUM_DECL, l, c), name(n) {}
    void accept(ASTVisitor& visitor);
};

struct EnumValueNode : ASTNode {
    std::string name;
    ASTPtr value; // optional integer expression
    
    EnumValueNode(const std::string& n, ASTPtr v, int l, int c)
        : ASTNode(NodeType::ENUM_VALUE, l, c), name(n), value(std::move(v)) {}
    void accept(ASTVisitor& visitor);
};

struct TypedefDeclNode : ASTNode {
    std::string source_type;
    std::string alias;
    
    TypedefDeclNode(const std::string& src, const std::string& al, int l, int c)
        : ASTNode(NodeType::TYPEDEF_DECL, l, c), source_type(src), alias(al) {}
    void accept(ASTVisitor& visitor);
};

struct SwitchStmtNode : ASTNode {
    ASTPtr condition;
    ASTPtr body;
    std::vector<ASTPtr> cases; // CaseLabelNode and DefaultLabelNode
    
    SwitchStmtNode(int l, int c) : ASTNode(NodeType::SWITCH_STMT, l, c) {}
    void accept(ASTVisitor& visitor);
};

struct CaseLabelNode : ASTNode {
    ASTPtr value;
    ASTPtr body;
    
    CaseLabelNode(int l, int c) : ASTNode(NodeType::CASE_LABEL, l, c) {}
    void accept(ASTVisitor& visitor);
};

struct DefaultLabelNode : ASTNode {
    ASTPtr body;
    
    DefaultLabelNode(int l, int c) : ASTNode(NodeType::DEFAULT_LABEL, l, c) {}
    void accept(ASTVisitor& visitor);
};

struct GotoStmtNode : ASTNode {
    std::string label;
    
    GotoStmtNode(const std::string& l, int line, int col)
        : ASTNode(NodeType::GOTO_STMT, line, col), label(l) {}
    void accept(ASTVisitor& visitor);
};

struct LabelStmtNode : ASTNode {
    std::string label;
    ASTPtr stmt;
    
    LabelStmtNode(const std::string& l, int line, int col)
        : ASTNode(NodeType::LABEL_STMT, line, col), label(l) {}
    void accept(ASTVisitor& visitor);
};

// Statements
struct BlockNode : ASTNode {
    std::vector<ASTPtr> statements;
    
    BlockNode(int l, int c) : ASTNode(NodeType::BLOCK, l, c) {}
    void accept(ASTVisitor& visitor);
};

struct ReturnStmtNode : ASTNode {
    ASTPtr value;
    
    ReturnStmtNode(int l, int c) : ASTNode(NodeType::RETURN_STMT, l, c) {}
    void accept(ASTVisitor& visitor);
};

struct ExprStmtNode : ASTNode {
    ASTPtr expr;
    
    ExprStmtNode(int l, int c) : ASTNode(NodeType::EXPR_STMT, l, c) {}
    void accept(ASTVisitor& visitor);
};

struct IfStmtNode : ASTNode {
    ASTPtr condition;
    ASTPtr then_branch;
    ASTPtr else_branch;
    
    IfStmtNode(int l, int c) : ASTNode(NodeType::IF_STMT, l, c) {}
    void accept(ASTVisitor& visitor);
};

struct WhileStmtNode : ASTNode {
    ASTPtr condition;
    ASTPtr body;
    
    WhileStmtNode(int l, int c) : ASTNode(NodeType::WHILE_STMT, l, c) {}
    void accept(ASTVisitor& visitor);
};

struct DoWhileStmtNode : ASTNode {
    ASTPtr body;
    ASTPtr condition;
    
    DoWhileStmtNode(int l, int c) : ASTNode(NodeType::DO_WHILE_STMT, l, c) {}
    void accept(ASTVisitor& visitor);
};

struct ForStmtNode : ASTNode {
    ASTPtr init;
    ASTPtr condition;
    ASTPtr update;
    ASTPtr body;
    
    ForStmtNode(int l, int c) : ASTNode(NodeType::FOR_STMT, l, c) {}
    void accept(ASTVisitor& visitor);
};

struct BreakStmtNode : ASTNode {
    BreakStmtNode(int l, int c) : ASTNode(NodeType::BREAK_STMT, l, c) {}
    void accept(ASTVisitor& visitor);
};

struct ContinueStmtNode : ASTNode {
    ContinueStmtNode(int l, int c) : ASTNode(NodeType::CONTINUE_STMT, l, c) {}
    void accept(ASTVisitor& visitor);
};

// Expressions
struct BinaryExprNode : ASTNode {
    OpKind op;
    ASTPtr left;
    ASTPtr right;
    
    BinaryExprNode(OpKind o, int l, int c) : ASTNode(NodeType::BINARY_EXPR, l, c), op(o) {}
    void accept(ASTVisitor& visitor);
};

struct UnaryExprNode : ASTNode {
    OpKind op;
    ASTPtr operand;
    
    UnaryExprNode(OpKind o, int l, int c) : ASTNode(NodeType::UNARY_EXPR, l, c), op(o) {}
    void accept(ASTVisitor& visitor);
};

struct AssignExprNode : ASTNode {
    ASTPtr target;
    ASTPtr value;
    
    AssignExprNode(int l, int c) : ASTNode(NodeType::ASSIGN_EXPR, l, c) {}
    void accept(ASTVisitor& visitor);
};

struct CompoundAssignExprNode : ASTNode {
    ASTPtr target;
    OpKind op;
    ASTPtr value;
    
    CompoundAssignExprNode(OpKind o, int l, int c)
        : ASTNode(NodeType::COMPOUND_ASSIGN_EXPR, l, c), op(o) {}
    void accept(ASTVisitor& visitor);
};

struct TernaryExprNode : ASTNode {
    ASTPtr condition;
    ASTPtr then_expr;
    ASTPtr else_expr;
    
    TernaryExprNode(int l, int c)
        : ASTNode(NodeType::TERNARY_EXPR, l, c) {}
    void accept(ASTVisitor& visitor);
};

struct CommaExprNode : ASTNode {
    ASTPtr left;
    ASTPtr right;
    
    CommaExprNode(int l, int c)
        : ASTNode(NodeType::COMMA_EXPR, l, c) {}
    void accept(ASTVisitor& visitor);
};

struct SizeofExprNode : ASTNode {
    std::string type_name;
    ASTPtr expr;
    bool is_type;
    
    SizeofExprNode(const std::string& type, int l, int c)
        : ASTNode(NodeType::SIZEOF_EXPR, l, c), type_name(type), is_type(true) {}
    SizeofExprNode(ASTPtr e, int l, int c)
        : ASTNode(NodeType::SIZEOF_EXPR, l, c), expr(std::move(e)), is_type(false) {}
    void accept(ASTVisitor& visitor);
};

struct CastExprNode : ASTNode {
    std::string target_type;
    ASTPtr expr;
    
    CastExprNode(const std::string& type, ASTPtr e, int l, int c)
        : ASTNode(NodeType::CAST_EXPR, l, c), target_type(type), expr(std::move(e)) {}
    void accept(ASTVisitor& visitor);
};

struct CallExprNode : ASTNode {
    std::string function_name;
    std::vector<ASTPtr> arguments;
    
    CallExprNode(const std::string& name, int l, int c)
        : ASTNode(NodeType::CALL_EXPR, l, c), function_name(name) {}
    void accept(ASTVisitor& visitor);
};

struct IndexExprNode : ASTNode {
    ASTPtr array;
    ASTPtr index;
    
    IndexExprNode(int l, int c) : ASTNode(NodeType::INDEX_EXPR, l, c) {}
    void accept(ASTVisitor& visitor);
};

struct MemberExprNode : ASTNode {
    ASTPtr object;
    std::string member;
    bool is_arrow;
    
    MemberExprNode(bool arrow, int l, int c)
        : ASTNode(NodeType::MEMBER_EXPR, l, c), is_arrow(arrow) {}
    void accept(ASTVisitor& visitor);
};

struct DerefExprNode : ASTNode {
    ASTPtr operand;
    
    DerefExprNode(int l, int c) : ASTNode(NodeType::DEREF_EXPR, l, c) {}
    void accept(ASTVisitor& visitor);
};

struct AddressOfExprNode : ASTNode {
    ASTPtr operand;
    
    AddressOfExprNode(int l, int c) : ASTNode(NodeType::ADDRESS_OF_EXPR, l, c) {}
    void accept(ASTVisitor& visitor);
};

struct StmtExprNode : ASTNode {
    ASTPtr body; // BlockNode containing statements, last one is the value
    
    StmtExprNode(int l, int c) : ASTNode(NodeType::STMT_EXPR, l, c) {}
    void accept(ASTVisitor& visitor);
};

// Literals
struct IntegerLiteralNode : ASTNode {
    long long value;
    
    IntegerLiteralNode(long long v, int l, int c)
        : ASTNode(NodeType::INTEGER_LITERAL, l, c), value(v) {}
    void accept(ASTVisitor& visitor);
};

struct FloatLiteralNode : ASTNode {
    double value;
    
    FloatLiteralNode(double v, int l, int c)
        : ASTNode(NodeType::FLOAT_LITERAL, l, c), value(v) {}
    void accept(ASTVisitor& visitor);
};

struct StringLiteralNode : ASTNode {
    std::string value;
    
    StringLiteralNode(const std::string& v, int l, int c)
        : ASTNode(NodeType::STRING_LITERAL, l, c), value(v) {}
    void accept(ASTVisitor& visitor);
};

struct CharLiteralNode : ASTNode {
    char value;
    
    CharLiteralNode(char v, int l, int c)
        : ASTNode(NodeType::CHAR_LITERAL, l, c), value(v) {}
    void accept(ASTVisitor& visitor);
};

struct IdentifierExprNode : ASTNode {
    std::string name;
    
    IdentifierExprNode(const std::string& n, int l, int c)
        : ASTNode(NodeType::IDENTIFIER_EXPR, l, c), name(n) {}
    void accept(ASTVisitor& visitor);
};

// Utility functions
const char* node_type_name(NodeType type);
const char* op_kind_name(OpKind op);

} // namespace simplecc
