#pragma once

#include "ast.h"
#include <string>
#include <sstream>
#include <map>
#include <vector>

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
    void visit(InitializerListNode& node) override;
    void visit(DesignatedInitNode& node) override;
    void visit(CompoundLiteralNode& node) override;
    
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
        bool is_extern;
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

    // Type tracking for the current expression context (used for float literal codegen)
    bool current_float_is_double_ = true;

    // Type stack: each subexpression pushes its result type so the parent can
    // decide whether to emit integer or SSE instructions.
    std::vector<std::string> expr_type_stack_;

    // Function signatures gathered in a pre-pass.
    std::map<std::string, std::string> function_return_type_;
    std::map<std::string, std::vector<std::string>> function_param_types_;

    // Return type of the function currently being generated.
    std::string current_function_return_type_;

    // Type inference helper
    std::string infer_expr_type(ASTNode* node);
    void push_expr_type(const std::string& t) { expr_type_stack_.push_back(t); }
    std::string pop_expr_type() {
        if (expr_type_stack_.empty()) return "int";
        std::string t = expr_type_stack_.back();
        expr_type_stack_.pop_back();
        return t;
    }
    static bool is_float_type(const std::string& t) {
        return t == "float" || t == "double" ||
               t == "const float" || t == "const double";
    }
    static bool is_double_type(const std::string& t) {
        return t == "double" || t == "const double";
    }

    // Struct layout helpers
    int get_struct_size(const std::string& name);
    int get_field_offset(const std::string& struct_name, const std::string& field_name);
    std::string get_field_type(const std::string& struct_name, const std::string& field_name);
    int get_type_size(const std::string& type);
    std::string get_struct_name(const std::string& type_name);
    void compute_member_address(MemberExprNode& node);
    std::string infer_member_expr_type(MemberExprNode& node);

    // Nested function support (GCC extension).
    //
    // A nested function's ABI is modified to take a hidden first argument
    // (in %rdi) that points to a struct containing the values of the
    // enclosing function's local variables that are captured by the
    // nested function.  This is a simpler alternative to GCC's
    // executable trampolines (which would require mprotect and break
    // modern W^X/NX protections).
    struct NestedFuncInfo {
        std::vector<FunctionDeclNode::CapturedVar> captures;
        int ctx_size = 0;  // total bytes for the context struct (aligned to 16)
    };
    std::map<std::string, NestedFuncInfo> nested_func_info_;

    // Captures for the function we are currently generating code for
    std::vector<FunctionDeclNode::CapturedVar> current_captures_;

    // Stack offset (relative to %rbp) where the hidden __ctx parameter
    // is saved in the current function.  Zero means "not a nested
    // function" (no __ctx present).
    int current_ctx_stack_offset_ = 0;

    // Returns the ctx_offset (offset within the context struct) of the
    // captured variable `name`, or -1 if `name` is not a capture in the
    // current function.
    int find_capture_offset(const std::string& name) const;

    // A nested function definition encountered inside another function's
    // body cannot be emitted inline (doing so would cause the enclosing
    // function to execute the nested function as part of its own body).
    // Instead, we record the nested function here and emit it after the
    // enclosing top-level function finishes its own body and epilogue.
    // The x86 `call` instruction uses a PC-relative offset, so forward
    // references are resolved by the assembler.
    struct PendingNestedFunc {
        FunctionDeclNode* node;
        std::vector<FunctionDeclNode::CapturedVar> captures;
        int ctx_size;
    };
    std::vector<PendingNestedFunc> pending_nested_functions_;

    // Emit a pending nested function (its prologue, body, epilogue).
    void emit_pending_nested_function(PendingNestedFunc& pnf);
};

} // namespace simplecc
