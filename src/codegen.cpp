#include "codegen.h"
#include <stdexcept>
#include <algorithm>

namespace simplecc {

CodeGenerator::CodeGenerator()
    : stack_offset_(0), label_counter_(0), returned_(false) {}

std::string CodeGenerator::generate(ProgramNode& program) {
    output_.str("");
    output_.clear();
    string_literals_.clear();
    global_variables_.clear();
    
    // First pass: collect global variables only
    for (auto& decl : program.declarations) {
        if (decl->type == NodeType::VAR_DECL) {
            auto* var = static_cast<VarDeclNode*>(decl.get());
            GlobalVar gvar;
            gvar.name = var->name;
            gvar.type = var->type_name;
            gvar.initialized = (var->initializer != nullptr);
            if (gvar.initialized && var->initializer->type == NodeType::INTEGER_LITERAL) {
                gvar.init_value = std::to_string(static_cast<IntegerLiteralNode*>(var->initializer.get())->value);
            }
            global_variables_.push_back(gvar);
        }
    }
    
    // Emit global variables
    if (!global_variables_.empty()) {
        emit(".data");
        for (const auto& gvar : global_variables_) {
            emit(".globl " + gvar.name);
            emit_label(gvar.name);
            if (gvar.initialized) {
                emit(".long " + gvar.init_value);
            } else {
                emit(".zero 4"); // default 4 bytes for int
            }
        }
    }
    
    // Emit string literals
    if (!string_literals_.empty()) {
        emit("");
        emit(".section .rodata");
        for (const auto& str : string_literals_) {
            emit_label(str.label);
            output_ << "    .asciz \"" << str.value << "\"\n";
        }
    }
    
    // Emit text section with functions
    emit("");
    emit(".text");
    for (auto& decl : program.declarations) {
        dispatch(decl.get());
    }
    
    return output_.str();
}

void CodeGenerator::emit(const std::string& line) {
    output_ << "    " << line << "\n";
}

void CodeGenerator::emit_label(const std::string& label) {
    output_ << label << ":\n";
}

void CodeGenerator::emit_function_prologue(const std::string& name) {
    emit(".globl " + name);
    emit_label(name);
    emit("push %rbp");
    emit("mov %rsp, %rbp");
    
    stack_offset_ = 0;
    local_variables_.clear();
    current_function_ = name;
}

void CodeGenerator::emit_function_epilogue() {
    emit("mov %rbp, %rsp");
    emit("pop %rbp");
    emit("ret");
    current_function_.clear();
}

void CodeGenerator::push(const std::string& reg) {
    emit("push " + reg);
}

void CodeGenerator::pop(const std::string& reg) {
    emit("pop " + reg);
}

std::string CodeGenerator::new_label(const std::string& prefix) {
    return "." + prefix + "_" + std::to_string(label_counter_++);
}

void CodeGenerator::dispatch(ASTNode* node) {
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
        case NodeType::PARAM:
            visit(static_cast<ParamNode&>(*node));
            break;
        case NodeType::BLOCK:
            visit(static_cast<BlockNode&>(*node));
            break;
        case NodeType::RETURN_STMT:
            visit(static_cast<ReturnStmtNode&>(*node));
            break;
        case NodeType::EXPR_STMT:
            visit(static_cast<ExprStmtNode&>(*node));
            break;
        case NodeType::IF_STMT:
            visit(static_cast<IfStmtNode&>(*node));
            break;
        case NodeType::WHILE_STMT:
            visit(static_cast<WhileStmtNode&>(*node));
            break;
        case NodeType::DO_WHILE_STMT:
            visit(static_cast<DoWhileStmtNode&>(*node));
            break;
        case NodeType::FOR_STMT:
            visit(static_cast<ForStmtNode&>(*node));
            break;
        case NodeType::BREAK_STMT:
            visit(static_cast<BreakStmtNode&>(*node));
            break;
        case NodeType::CONTINUE_STMT:
            visit(static_cast<ContinueStmtNode&>(*node));
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
        case NodeType::COMPOUND_ASSIGN_EXPR:
            visit(static_cast<CompoundAssignExprNode&>(*node));
            break;
        case NodeType::TERNARY_EXPR:
            visit(static_cast<TernaryExprNode&>(*node));
            break;
        case NodeType::COMMA_EXPR:
            visit(static_cast<CommaExprNode&>(*node));
            break;
        case NodeType::SIZEOF_EXPR:
            visit(static_cast<SizeofExprNode&>(*node));
            break;
        case NodeType::CAST_EXPR:
            visit(static_cast<CastExprNode&>(*node));
            break;
        case NodeType::CALL_EXPR:
            visit(static_cast<CallExprNode&>(*node));
            break;
        case NodeType::INDEX_EXPR:
            visit(static_cast<IndexExprNode&>(*node));
            break;
        case NodeType::MEMBER_EXPR:
            visit(static_cast<MemberExprNode&>(*node));
            break;
        case NodeType::DEREF_EXPR:
            visit(static_cast<DerefExprNode&>(*node));
            break;
        case NodeType::ADDRESS_OF_EXPR:
            visit(static_cast<AddressOfExprNode&>(*node));
            break;
        case NodeType::INTEGER_LITERAL:
            visit(static_cast<IntegerLiteralNode&>(*node));
            break;
        case NodeType::FLOAT_LITERAL:
            visit(static_cast<FloatLiteralNode&>(*node));
            break;
        case NodeType::STRING_LITERAL:
            visit(static_cast<StringLiteralNode&>(*node));
            break;
        case NodeType::CHAR_LITERAL:
            visit(static_cast<CharLiteralNode&>(*node));
            break;
        case NodeType::IDENTIFIER_EXPR:
            visit(static_cast<IdentifierExprNode&>(*node));
            break;
    }
}

// Visitor implementations

void CodeGenerator::visit(ProgramNode& node) {
    // First pass: collect global variables
    for (auto& decl : node.declarations) {
        if (decl->type == NodeType::VAR_DECL) {
            auto* var = static_cast<VarDeclNode*>(decl.get());
            GlobalVar gvar;
            gvar.name = var->name;
            gvar.type = var->type_name;
            gvar.initialized = (var->initializer != nullptr);
            if (gvar.initialized && var->initializer->type == NodeType::INTEGER_LITERAL) {
                gvar.init_value = std::to_string(static_cast<IntegerLiteralNode*>(var->initializer.get())->value);
            }
            global_variables_.push_back(gvar);
        }
    }
    
    // Second pass: generate code
    for (auto& decl : node.declarations) {
        dispatch(decl.get());
    }
}

void CodeGenerator::visit(FunctionDeclNode& node) {
    // Skip forward declarations (no body)
    if (!node.body) {
        return;
    }
    
    returned_ = false;
    current_function_ = node.name;
    emit_function_prologue(node.name);
    
    // Map parameters to stack slots (System V ABI: rdi, rsi, rdx, rcx, r8, r9)
    static const char* param_regs[] = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};
    int num_params = std::min((int)node.params.size(), 6);
    
    // Count local variables to pre-allocate stack space
    int local_count = 0;
    if (node.body) {
        auto& block = static_cast<BlockNode&>(*node.body);
        for (auto& stmt : block.statements) {
            if (stmt->type == NodeType::VAR_DECL) {
                local_count++;
            }
        }
    }
    int total_slots = num_params + local_count + 16;  // 16 extra slots for expression temps
    if (total_slots > 0) {
        int space = total_slots * 8;
        emit("sub $" + std::to_string(space) + ", %rsp");
    }
    
    for (int i = 0; i < num_params; i++) {
        stack_offset_ += 8;
        int offset = -stack_offset_;
        local_variables_[static_cast<ParamNode*>(node.params[i].get())->name] = offset;
        emit("mov " + std::string(param_regs[i]) + ", " + std::to_string(offset) + "(%rbp)");
    }
    
    if (node.body) {
        dispatch(node.body.get());
    }
    
    if (!returned_) {
        emit("mov $0, %rax");
        emit_function_epilogue();
    }
}

void CodeGenerator::visit(VarDeclNode& node) {
    // Global variables are handled in the first pass
    // Only process local variables here (inside functions)
    if (current_function_.empty()) {
        return; // Skip global variables in code generation pass
    }
    
    stack_offset_ += 8;
    local_variables_[node.name] = -stack_offset_;
    
    if (node.initializer) {
        dispatch(node.initializer.get());
        emit("mov %rax, " + std::to_string(-stack_offset_) + "(%rbp)");
    }
}

void CodeGenerator::visit(ParamNode& node) {
    // Parameters are passed in registers
}

void CodeGenerator::visit(BlockNode& node) {
    for (auto& stmt : node.statements) {
        dispatch(stmt.get());
    }
}

void CodeGenerator::visit(ReturnStmtNode& node) {
    if (node.value) {
        dispatch(node.value.get());
    } else {
        emit("mov $0, %rax");
    }
    emit("mov %rbp, %rsp");
    emit("pop %rbp");
    emit("ret");
    returned_ = true;
}

void CodeGenerator::visit(ExprStmtNode& node) {
    if (node.expr) {
        dispatch(node.expr.get());
    }
}

void CodeGenerator::visit(IfStmtNode& node) {
    std::string else_label = new_label("else");
    std::string end_label = new_label("endif");
    
    dispatch(node.condition.get());
    emit("cmp $0, %rax");
    emit("je " + else_label);
    
    if (node.then_branch) {
        dispatch(node.then_branch.get());
    }
    
    if (node.else_branch) {
        emit("jmp " + end_label);
    }
    
    emit_label(else_label);
    
    if (node.else_branch) {
        dispatch(node.else_branch.get());
    }
    
    emit_label(end_label);
}

void CodeGenerator::visit(WhileStmtNode& node) {
    std::string start_label = new_label("while_start");
    std::string end_label = new_label("while_end");
    
    emit_label(start_label);
    
    dispatch(node.condition.get());
    emit("cmp $0, %rax");
    emit("je " + end_label);
    
    if (node.body) {
        dispatch(node.body.get());
    }
    
    emit("jmp " + start_label);
    emit_label(end_label);
}

void CodeGenerator::visit(DoWhileStmtNode& node) {
    std::string start_label = new_label("do_while_start");
    std::string end_label = new_label("do_while_end");
    
    emit_label(start_label);
    
    if (node.body) {
        dispatch(node.body.get());
    }
    
    dispatch(node.condition.get());
    emit("cmp $0, %rax");
    emit("jne " + start_label);
    
    emit_label(end_label);
}

void CodeGenerator::visit(ForStmtNode& node) {
    std::string start_label = new_label("for_start");
    std::string cond_label = new_label("for_cond");
    std::string end_label = new_label("for_end");
    
    if (node.init) {
        dispatch(node.init.get());
    }
    
    emit("jmp " + cond_label);
    emit_label(start_label);
    
    if (node.body) {
        dispatch(node.body.get());
    }
    
    if (node.update) {
        dispatch(node.update.get());
    }
    
    emit_label(cond_label);
    
    if (node.condition) {
        dispatch(node.condition.get());
        emit("cmp $0, %rax");
        emit("jne " + start_label);
    } else {
        emit("jmp " + start_label);
    }
    
    emit_label(end_label);
}

void CodeGenerator::visit(BreakStmtNode& node) {
    emit("# break");
}

void CodeGenerator::visit(ContinueStmtNode& node) {
    emit("# continue");
}

void CodeGenerator::visit(BinaryExprNode& node) {
    generate_binary(node);
}

void CodeGenerator::visit(UnaryExprNode& node) {
    generate_unary(node);
}

void CodeGenerator::visit(AssignExprNode& node) {
    dispatch(node.value.get());
    
    if (auto* id = dynamic_cast<IdentifierExprNode*>(node.target.get())) {
        if (local_variables_.count(id->name)) {
            int offset = local_variables_[id->name];
            emit("mov %rax, " + std::to_string(offset) + "(%rbp)");
        }
    }
}

void CodeGenerator::visit(CompoundAssignExprNode& node) {
    // Load current value of target
    if (auto* id = dynamic_cast<IdentifierExprNode*>(node.target.get())) {
        if (local_variables_.count(id->name)) {
            int offset = local_variables_[id->name];
            emit("mov " + std::to_string(offset) + "(%rbp), %rax");
        }
    }
    
    // Save current value
    emit("push %rax");
    
    // Evaluate right operand
    dispatch(node.value.get());
    
    // Move right to rcx
    emit("mov %rax, %rcx");
    
    // Pop left (current value) to rax
    emit("pop %rax");
    
    // Apply operator
    switch (node.op) {
        case OpKind::ADD: emit("add %rcx, %rax"); break;
        case OpKind::SUB: emit("sub %rcx, %rax"); break;
        case OpKind::MUL: emit("imul %rcx, %rax"); break;
        case OpKind::DIV: emit("cqo"); emit("idiv %rcx"); break;
        case OpKind::MOD: emit("cqo"); emit("idiv %rcx"); emit("mov %rdx, %rax"); break;
        case OpKind::BIT_AND: emit("and %rcx, %rax"); break;
        case OpKind::BIT_OR: emit("or %rcx, %rax"); break;
        case OpKind::BIT_XOR: emit("xor %rcx, %rax"); break;
        case OpKind::LSHIFT: emit("shl %cl, %rax"); break;
        case OpKind::RSHIFT: emit("shr %cl, %rax"); break;
        default: break;
    }
    
    // Store result back
    if (auto* id = dynamic_cast<IdentifierExprNode*>(node.target.get())) {
        if (local_variables_.count(id->name)) {
            int offset = local_variables_[id->name];
            emit("mov %rax, " + std::to_string(offset) + "(%rbp)");
        }
    }
}

void CodeGenerator::visit(TernaryExprNode& node) {
    std::string else_label = new_label("ternary_else");
    std::string end_label = new_label("ternary_end");
    
    // Evaluate condition
    dispatch(node.condition.get());
    emit("cmp $0, %rax");
    emit("je " + else_label);
    
    // Then expression
    dispatch(node.then_expr.get());
    emit("jmp " + end_label);
    
    // Else expression
    emit_label(else_label);
    dispatch(node.else_expr.get());
    
    emit_label(end_label);
}

void CodeGenerator::visit(CommaExprNode& node) {
    dispatch(node.left.get());
    dispatch(node.right.get());
}

void CodeGenerator::visit(SizeofExprNode& node) {
    // Simple sizeof implementation
    // Returns 4 for int, 1 for char, 8 for pointers, etc.
    if (node.is_type) {
        if (node.type_name == "int" || node.type_name == "const int") {
            emit("mov $4, %rax");
        } else if (node.type_name == "char" || node.type_name == "const char") {
            emit("mov $1, %rax");
        } else if (node.type_name == "bool") {
            emit("mov $1, %rax");
        } else if (node.type_name == "void") {
            emit("mov $1, %rax");
        } else {
            emit("mov $8, %rax"); // default to 8 bytes
        }
    } else {
        // sizeof expression - evaluate and return size based on type
        dispatch(node.expr.get());
        emit("mov $8, %rax"); // simplified: assume 8 bytes
    }
}

void CodeGenerator::visit(CastExprNode& node) {
    // Type cast - currently just evaluate the expression
    // Full implementation would check type compatibility
    dispatch(node.expr.get());
}

void CodeGenerator::visit(CallExprNode& node) {
    static const char* param_regs[] = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};
    
    // Evaluate arguments and push onto stack (right to left)
    int num_args = std::min((int)node.arguments.size(), 6);
    for (int i = num_args - 1; i >= 0; i--) {
        dispatch(node.arguments[i].get());
        emit("push %rax");
    }
    
    // Pop arguments into registers (left to right)
    for (int i = 0; i < num_args; i++) {
        emit("pop " + std::string(param_regs[i]));
    }
    
    emit("call " + node.function_name);
}

void CodeGenerator::visit(IndexExprNode& node) {
    // Load base address
    dispatch(node.array.get());
    emit("push %rax");
    
    // Load index
    dispatch(node.index.get());
    
    // Compute address: base + index * element_size
    // For now, assume char (1 byte)
    emit("pop %rcx");
    emit("add %rcx, %rax");
    
    // Load value at address
    emit("movzbl (%rax), %eax");
}

void CodeGenerator::visit(MemberExprNode& node) {
    emit("# member");
}

void CodeGenerator::visit(DerefExprNode& node) {
    dispatch(node.operand.get());
    emit("mov (%rax), %rax");
}

void CodeGenerator::visit(AddressOfExprNode& node) {
    if (auto* id = dynamic_cast<IdentifierExprNode*>(node.operand.get())) {
        if (local_variables_.count(id->name)) {
            int offset = local_variables_[id->name];
            emit("lea " + std::to_string(offset) + "(%rbp), %rax");
        }
    }
}

void CodeGenerator::visit(IntegerLiteralNode& node) {
    emit("mov $" + std::to_string(node.value) + ", %rax");
}

void CodeGenerator::visit(FloatLiteralNode& node) {
    emit("# float " + std::to_string(node.value));
}

void CodeGenerator::visit(StringLiteralNode& node) {
    // Add string to literal table and emit address
    std::string label = ".Lstr_" + std::to_string(string_literals_.size());
    string_literals_.push_back({label, node.value});
    
    // Load address of string into rax
    emit("lea " + label + "(%rip), %rax");
}

void CodeGenerator::visit(CharLiteralNode& node) {
    emit("mov $" + std::to_string(static_cast<int>(node.value)) + ", %rax");
}

void CodeGenerator::visit(IdentifierExprNode& node) {
    if (local_variables_.count(node.name)) {
        int offset = local_variables_[node.name];
        emit("mov " + std::to_string(offset) + "(%rbp), %rax");
    } else {
        // Check if it's a global variable
        for (const auto& gvar : global_variables_) {
            if (gvar.name == node.name) {
                emit("mov " + node.name + "(%rip), %rax");
                return;
            }
        }
        // Check if it's a function
        emit("lea " + node.name + "(%rip), %rax");
    }
}

// Helper methods

void CodeGenerator::generate_expression(ASTNode* node) {
    if (node) {
        dispatch(node);
    }
}

void CodeGenerator::generate_binary(BinaryExprNode& node) {
    // Evaluate right operand, save on stack
    dispatch(node.right.get());
    emit("push %rax");
    
    // Evaluate left operand into rax
    dispatch(node.left.get());
    
    // Restore right operand into rcx
    emit("pop %rcx");
    
    // Apply operator: rax = left op right
    // rcx has right, rax has left
    
    switch (node.op) {
        case OpKind::ADD:
            emit("add %rcx, %rax");
            break;
        case OpKind::SUB:
            emit("sub %rcx, %rax");
            break;
        case OpKind::MUL:
            emit("imul %rcx, %rax");
            break;
        case OpKind::DIV:
            emit("cqo");
            emit("idiv %rcx");
            break;
        case OpKind::MOD:
            emit("cqo");
            emit("idiv %rcx");
            emit("mov %rdx, %rax");
            break;
        case OpKind::EQ:
            emit("cmp %rcx, %rax");
            emit("sete %al");
            emit("movzbq %al, %rax");
            break;
        case OpKind::NE:
            emit("cmp %rcx, %rax");
            emit("setne %al");
            emit("movzbq %al, %rax");
            break;
        case OpKind::LT:
            emit("cmp %rcx, %rax");
            emit("setl %al");
            emit("movzbq %al, %rax");
            break;
        case OpKind::GT:
            emit("cmp %rcx, %rax");
            emit("setg %al");
            emit("movzbq %al, %rax");
            break;
        case OpKind::LE:
            emit("cmp %rcx, %rax");
            emit("setle %al");
            emit("movzbq %al, %rax");
            break;
        case OpKind::GE:
            emit("cmp %rcx, %rax");
            emit("setge %al");
            emit("movzbq %al, %rax");
            break;
        case OpKind::AND: {
            std::string false_label = new_label("and_false");
            std::string end_label = new_label("and_end");
            emit("cmp $0, %rax");
            emit("je " + false_label);
            emit("cmp $0, %rcx");
            emit("je " + false_label);
            emit("mov $1, %rax");
            emit("jmp " + end_label);
            emit_label(false_label);
            emit("mov $0, %rax");
            emit_label(end_label);
            break;
        }
        case OpKind::OR: {
            std::string true_label = new_label("or_true");
            std::string end_label = new_label("or_end");
            emit("cmp $0, %rax");
            emit("jne " + true_label);
            emit("cmp $0, %rcx");
            emit("jne " + true_label);
            emit("mov $0, %rax");
            emit("jmp " + end_label);
            emit_label(true_label);
            emit("mov $1, %rax");
            emit_label(end_label);
            break;
        }
        case OpKind::BIT_AND:
            emit("and %rcx, %rax");
            break;
        case OpKind::BIT_OR:
            emit("or %rcx, %rax");
            break;
        case OpKind::BIT_XOR:
            emit("xor %rcx, %rax");
            break;
        case OpKind::LSHIFT:
            emit("shl %cl, %rax");
            break;
        case OpKind::RSHIFT:
            emit("shr %cl, %rax");
            break;
        default:
            error_message_ = "Unsupported binary operator";
            break;
    }
}

void CodeGenerator::generate_unary(UnaryExprNode& node) {
    dispatch(node.operand.get());
    
    switch (node.op) {
        case OpKind::UMINUS:
            emit("neg %rax");
            break;
        case OpKind::UPLUS:
            break;
        case OpKind::NOT:
            emit("cmp $0, %rax");
            emit("sete %al");
            emit("movzbq %al, %rax");
            break;
        case OpKind::BIT_NOT:
            emit("not %rax");
            break;
        case OpKind::DEREF:
            emit("mov (%rax), %rax");
            break;
        case OpKind::ADDRESS_OF:
            emit("# address-of");
            break;
        case OpKind::PRE_INC:
            emit("# pre-inc");
            break;
        case OpKind::PRE_DEC:
            emit("# pre-dec");
            break;
        case OpKind::POST_INC:
            // Load current value
            if (auto* id = dynamic_cast<IdentifierExprNode*>(node.operand.get())) {
                if (local_variables_.count(id->name)) {
                    int offset = local_variables_[id->name];
                    emit("mov " + std::to_string(offset) + "(%rbp), %rax");
                    emit("push %rax");
                    emit("add $1, %rax");
                    emit("mov %rax, " + std::to_string(offset) + "(%rbp)");
                    emit("pop %rax");
                }
            }
            break;
        case OpKind::POST_DEC:
            // Load current value
            if (auto* id = dynamic_cast<IdentifierExprNode*>(node.operand.get())) {
                if (local_variables_.count(id->name)) {
                    int offset = local_variables_[id->name];
                    emit("mov " + std::to_string(offset) + "(%rbp), %rax");
                    emit("push %rax");
                    emit("sub $1, %rax");
                    emit("mov %rax, " + std::to_string(offset) + "(%rbp)");
                    emit("pop %rax");
                }
            }
            break;
        default:
            error_message_ = "Unsupported unary operator";
            break;
    }
}

} // namespace simplecc
