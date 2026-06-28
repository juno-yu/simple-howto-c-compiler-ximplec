#include "codegen.h"
#include <stdexcept>
#include <algorithm>

namespace simplecc {

CodeGenerator::CodeGenerator()
    : stack_offset_(0), label_counter_(0), returned_(false) {}

std::string CodeGenerator::generate(ProgramNode& program) {
    output_.str("");
    output_.clear();
    
    emit(".text");
    dispatch(&program);
    
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
    for (auto& decl : node.declarations) {
        dispatch(decl.get());
    }
}

void CodeGenerator::visit(FunctionDeclNode& node) {
    returned_ = false;
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

void CodeGenerator::visit(CallExprNode& node) {
    static const char* param_regs[] = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};
    
    // Evaluate arguments in reverse order (right to left)
    int num_args = std::min((int)node.arguments.size(), 6);
    for (int i = num_args - 1; i >= 0; i--) {
        dispatch(node.arguments[i].get());
        emit("mov %rax, " + std::string(param_regs[i]));
    }
    
    emit("call " + node.function_name);
}

void CodeGenerator::visit(IndexExprNode& node) {
    emit("# index");
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
    emit("# string");
}

void CodeGenerator::visit(CharLiteralNode& node) {
    emit("mov $" + std::to_string(static_cast<int>(node.value)) + ", %rax");
}

void CodeGenerator::visit(IdentifierExprNode& node) {
    if (local_variables_.count(node.name)) {
        int offset = local_variables_[node.name];
        emit("mov " + std::to_string(offset) + "(%rbp), %rax");
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
            emit("# post-inc");
            break;
        case OpKind::POST_DEC:
            emit("# post-dec");
            break;
        default:
            error_message_ = "Unsupported unary operator";
            break;
    }
}

} // namespace simplecc
