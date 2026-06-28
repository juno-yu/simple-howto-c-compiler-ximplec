#pragma once

#include "ast.h"
#include <string>
#include <sstream>
#include <map>

namespace simplecc {

class CodeGenerator : public ASTVisitor {
public:
    CodeGenerator();
    ~CodeGenerator() = default;
    
    std::string generate(ProgramNode& program);
    
    bool has_error() const { return !error_message_.empty(); }
    const std::string& error_message() const { return error_message_; }
    
private:
    // Visitor implementations
    void visit(ProgramNode& node) override;
    void visit(FunctionDeclNode& node) override;
    void visit(VarDeclNode& node) override;
    void visit(StructFieldNode& node) override;
    void visit(StructDeclNode& node) override;
    void visit(EnumDeclNode& node) override;
    void visit(EnumValueNode& node) override;
    void visit(TypedefDeclNode& node) override;
    void visit(SwitchStmtNode& node) override;
    void visit(CaseLabelNode& node) override;
    void visit(DefaultLabelNode& node) override;
    void visit(GotoStmtNode& node) override;
    void visit(LabelStmtNode& node) override;
    void visit(ParamNode& node) override;
    void visit(BlockNode& node) override;
    void visit(ReturnStmtNode& node) override;
    void visit(ExprStmtNode& node) override;
    void visit(IfStmtNode& node) override;
    void visit(WhileStmtNode& node) override;
    void visit(DoWhileStmtNode& node) override;
    void visit(ForStmtNode& node) override;
    void visit(BreakStmtNode& node) override;
    void visit(ContinueStmtNode& node) override;
    void visit(BinaryExprNode& node) override;
    void visit(UnaryExprNode& node) override;
    void visit(AssignExprNode& node) override;
    void visit(CompoundAssignExprNode& node) override;
    void visit(TernaryExprNode& node) override;
    void visit(CommaExprNode& node) override;
    void visit(SizeofExprNode& node) override;
    void visit(CastExprNode& node) override;
    void visit(CallExprNode& node) override;
    void visit(IndexExprNode& node) override;
    void visit(MemberExprNode& node) override;
    void visit(DerefExprNode& node) override;
    void visit(AddressOfExprNode& node) override;
    void visit(StmtExprNode& node) override;
    void visit(AsmStmtNode& node) override;
    void visit(IntegerLiteralNode& node) override;
    void visit(FloatLiteralNode& node) override;
    void visit(StringLiteralNode& node) override;
    void visit(CharLiteralNode& node) override;
    void visit(IdentifierExprNode& node) override;
    
    // Code generation helpers
    void emit(const std::string& line);
    void emit_label(const std::string& label);
    void emit_function_prologue(const std::string& name);
    void emit_function_epilogue();
    
    void push(const std::string& reg = "%rax");
    void pop(const std::string& reg = "%rax");
    
    void generate_expression(ASTNode* node);
    void generate_binary(BinaryExprNode& node);
    void generate_unary(UnaryExprNode& node);
    void dispatch(ASTNode* node);
    
    std::string new_label(const std::string& prefix);
    
    std::stringstream output_;
    std::string error_message_;
    
    // Stack frame tracking
    int stack_offset_;
    std::map<std::string, int> local_variables_;
    
    // Label counters
    int label_counter_;
    
    // Current function context
    std::string current_function_;
    
    // Track if current function has returned
    bool returned_;
    
    // Switch statement context
    std::string current_case_label_;
    std::string current_switch_end_;
    
    // Loop context for break/continue
    std::string current_loop_start_;
    std::string current_loop_end_;
    
    // String literals for .rodata section
    struct StringLiteral {
        std::string label;
        std::string value;
    };
    std::vector<StringLiteral> string_literals_;
    
    // Global variables
    struct GlobalVar {
        std::string name;
        std::string type;
        bool initialized;
        std::string init_value;
    };
    std::vector<GlobalVar> global_variables_;
    
    // Struct layouts for member access
    struct FieldInfo {
        std::string name;
        std::string type;
        int offset;
        int size;
    };
    std::map<std::string, std::vector<FieldInfo>> struct_layouts_;
    
    // Enum values
    std::map<std::string, int> enum_values_;
    
    // Typedef mappings
    std::map<std::string, std::string> typedef_map_;
    
    // Variable type tracking (for struct member access)
    std::map<std::string, std::string> variable_types_;
    
    // Array info (element size, array length)
    struct ArrayInfo {
        int elem_size;
        int length;
    };
    std::map<std::string, ArrayInfo> array_info_;
    
    // Struct layout helpers
    int get_struct_size(const std::string& name);
    int get_field_offset(const std::string& struct_name, const std::string& field_name);
    int get_type_size(const std::string& type);
    void compute_member_address(MemberExprNode& node);
};

} // namespace simplecc
