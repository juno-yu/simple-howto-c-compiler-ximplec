#include "codegen.h"
#include <stdexcept>
#include <algorithm>
#include <cstring>
#include <cstdint>
#include <functional>

namespace simplecc {

CodeGenerator::CodeGenerator()
    : stack_offset_(0), label_counter_(0), returned_(false) {}

std::string CodeGenerator::generate(ProgramNode& program) {
    output_.str("");
    output_.clear();
    string_literals_.clear();
    global_variables_.clear();
    function_return_type_.clear();
    function_param_types_.clear();

    // First pass: collect global variables and function signatures (for ABI)
    for (auto& decl : program.declarations) {
        if (decl->type == NodeType::VAR_DECL) {
            auto* var = static_cast<VarDeclNode*>(decl.get());
            GlobalVar gvar;
            gvar.name = var->name;
            gvar.type = var->type_name;
            gvar.initialized = (var->initializer != nullptr);
            gvar.is_extern = var->is_extern;
            if (gvar.initialized && var->initializer->type == NodeType::INTEGER_LITERAL) {
                gvar.init_value = std::to_string(static_cast<IntegerLiteralNode*>(var->initializer.get())->value);
            }
            global_variables_.push_back(gvar);
        } else if (decl->type == NodeType::FUNCTION_DECL) {
            auto* fn = static_cast<FunctionDeclNode*>(decl.get());
            function_return_type_[fn->name] = fn->return_type;
            std::vector<std::string> param_types;
            for (auto& p : fn->params) {
                if (auto* pn = dynamic_cast<ParamNode*>(p.get())) {
                    param_types.push_back(pn->type_name);
                }
            }
            function_param_types_[fn->name] = std::move(param_types);
        }
    }
    
    // Emit global variables (skip extern - they're defined elsewhere)
    bool has_non_extern_globals = false;
    for (const auto& gvar : global_variables_) {
        if (!gvar.is_extern) {
            has_non_extern_globals = true;
            break;
        }
    }
    
    if (has_non_extern_globals) {
        emit(".data");
        for (const auto& gvar : global_variables_) {
            if (gvar.is_extern) continue; // Skip extern variables
            emit(".globl " + gvar.name);
            emit_label(gvar.name);
            if (gvar.initialized) {
                emit(".long " + gvar.init_value);
            } else {
                emit(".zero 4");
            }
        }
    }
    
    // Emit text section with functions
    emit("");
    emit(".text");
    for (auto& decl : program.declarations) {
        dispatch(decl.get());
    }
    
    // Emit string literals (collected during codegen)
    if (!string_literals_.empty()) {
        emit("");
        emit(".section .rodata");
        for (const auto& str : string_literals_) {
            emit_label(str.label);
            output_ << "    .asciz \"" << str.value << "\"\n";
        }
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
        case NodeType::STRUCT_FIELD:
            visit(static_cast<StructFieldNode&>(*node));
            break;
        case NodeType::STRUCT_DECL:
            visit(static_cast<StructDeclNode&>(*node));
            break;
        case NodeType::ENUM_DECL:
            visit(static_cast<EnumDeclNode&>(*node));
            break;
        case NodeType::ENUM_VALUE:
            visit(static_cast<EnumValueNode&>(*node));
            break;
        case NodeType::TYPEDEF_DECL:
            visit(static_cast<TypedefDeclNode&>(*node));
            break;
        case NodeType::SWITCH_STMT:
            visit(static_cast<SwitchStmtNode&>(*node));
            break;
        case NodeType::CASE_LABEL:
            visit(static_cast<CaseLabelNode&>(*node));
            break;
        case NodeType::DEFAULT_LABEL:
            visit(static_cast<DefaultLabelNode&>(*node));
            break;
        case NodeType::GOTO_STMT:
            visit(static_cast<GotoStmtNode&>(*node));
            break;
        case NodeType::LABEL_STMT:
            visit(static_cast<LabelStmtNode&>(*node));
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
        case NodeType::STMT_EXPR:
            visit(static_cast<StmtExprNode&>(*node));
            break;
        case NodeType::ASM_STMT:
            visit(static_cast<AsmStmtNode&>(*node));
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
        case NodeType::INITIALIZER_LIST:
            visit(static_cast<InitializerListNode&>(*node));
            break;
        case NodeType::DESIGNATED_INIT:
            visit(static_cast<DesignatedInitNode&>(*node));
            break;
        case NodeType::COMPOUND_LITERAL:
            visit(static_cast<CompoundLiteralNode&>(*node));
            break;
    }
}

// Visitor implementations

void CodeGenerator::visit(ProgramNode& node) {
    // First pass: collect global variables and function signatures (for ABI)
    for (auto& decl : node.declarations) {
        if (decl->type == NodeType::VAR_DECL) {
            auto* var = static_cast<VarDeclNode*>(decl.get());
            GlobalVar gvar;
            gvar.name = var->name;
            gvar.type = var->type_name;
            gvar.initialized = (var->initializer != nullptr);
            gvar.is_extern = var->is_extern;
            if (gvar.initialized && var->initializer->type == NodeType::INTEGER_LITERAL) {
                gvar.init_value = std::to_string(static_cast<IntegerLiteralNode*>(var->initializer.get())->value);
            }
            global_variables_.push_back(gvar);
        } else if (decl->type == NodeType::FUNCTION_DECL) {
            auto* fn = static_cast<FunctionDeclNode*>(decl.get());
            function_return_type_[fn->name] = fn->return_type;
            std::vector<std::string> param_types;
            for (auto& p : fn->params) {
                if (auto* pn = dynamic_cast<ParamNode*>(p.get())) {
                    param_types.push_back(pn->type_name);
                }
            }
            function_param_types_[fn->name] = std::move(param_types);
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

    if (node.is_nested) {
        // Nested function (GCC extension).  We use a hidden context pointer
        // ABI: the first argument (in %rdi) is a pointer to a struct
        // containing the values of the enclosing function's local variables
        // that are captured by this nested function.
        //
        // The function body cannot be emitted inline in the enclosing
        // function's code stream, because that would cause the enclosing
        // function to execute the nested function as part of its own body
        // (and also corrupt the stack frame).  Instead, we record the
        // nested function for emission after the enclosing top-level
        // function finishes its own body and epilogue.  The `call`
        // instruction uses a PC-relative offset, so the forward reference
        // is resolved by the assembler.

        // Snapshot the enclosing function's state.  The enclosing
        // function's local_variables_ is the candidate set for capture.
        auto saved_locals = local_variables_;
        auto saved_types = variable_types_;
        auto saved_arrays = array_info_;
        auto saved_captures = current_captures_;
        int saved_ctx_stack_offset = current_ctx_stack_offset_;

        // Compute the capture set: every local variable of the enclosing
        // function that exists at this point.
        std::vector<FunctionDeclNode::CapturedVar> captures;
        int ctx_off = 0;
        for (const auto& kv : saved_locals) {
            FunctionDeclNode::CapturedVar c;
            c.name = kv.first;
            c.parent_offset = kv.second;
            auto tit = saved_types.find(kv.first);
            c.type = (tit != saved_types.end()) ? tit->second : "int";
            c.size = get_type_size(c.type);
            c.ctx_offset = ctx_off;
            captures.push_back(c);
            ctx_off += 8;  // 8 bytes per slot
        }
        node.captured_vars = captures;

        int ctx_size = captures.size() * 8;
        if (ctx_size < 16) ctx_size = 16;
        ctx_size = ((ctx_size + 15) / 16) * 16;

        NestedFuncInfo info;
        info.captures = captures;
        info.ctx_size = ctx_size;
        nested_func_info_[node.name] = info;

        // Defer the body emission.
        PendingNestedFunc pnf;
        pnf.node = &node;
        pnf.captures = captures;
        pnf.ctx_size = ctx_size;
        pending_nested_functions_.push_back(pnf);

        // Restore enclosing function's state (no codegen output here)
        local_variables_ = saved_locals;
        variable_types_ = saved_types;
        array_info_ = saved_arrays;
        current_captures_ = saved_captures;
        current_ctx_stack_offset_ = saved_ctx_stack_offset;
        return;
    }

    // Top-level (non-nested) function: original logic.
    returned_ = false;
    current_function_ = node.name;
    current_function_return_type_ = node.return_type;
    current_captures_.clear();
    current_ctx_stack_offset_ = 0;
    emit_function_prologue(node.name);

    // Map parameters to stack slots.  System V x86-64 ABI: integer/pointer
    // args go in %rdi..%r9 (in order); float/double args go in %xmm0..%xmm7
    // (in order).  Mixing is allowed.
    static const char* param_regs[] = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};
    int num_params = (int)node.params.size();

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

    int int_idx = 0;
    int xmm_idx = 0;
    for (int i = 0; i < num_params; i++) {
        auto* pn = static_cast<ParamNode*>(node.params[i].get());
        std::string ptype = pn->type_name;
        bool is_fp = is_float_type(ptype);
        bool is_dbl = is_double_type(ptype);
        int psize = get_type_size(ptype);
        stack_offset_ += psize > 4 ? 8 : 8;  // always 8 bytes for the slot
        int offset = -stack_offset_;
        local_variables_[pn->name] = offset;
        variable_types_[pn->name] = ptype;
        if (is_fp) {
            std::string xreg = "%xmm" + std::to_string(xmm_idx++);
            std::string mop = is_dbl ? "movsd" : "movss";
            emit(mop + " " + xreg + ", " + std::to_string(offset) + "(%rbp)");
        } else {
            if (int_idx < 6) {
                emit("mov " + std::string(param_regs[int_idx++]) + ", " + std::to_string(offset) + "(%rbp)");
            } else {
                int_idx++;
            }
        }
    }

    if (node.body) {
        dispatch(node.body.get());
    }

    if (!returned_) {
        if (is_float_type(current_function_return_type_)) {
            if (is_double_type(current_function_return_type_)) {
                emit("xorpd %xmm0, %xmm0");
            } else {
                emit("xorps %xmm0, %xmm0");
            }
        } else {
            emit("mov $0, %rax");
        }
        emit_function_epilogue();
    }

    // Emit any nested functions that were defined inside this function's
    // body.  We do this after the enclosing function's epilogue so that
    // the nested function's code is not executed inline as part of the
    // enclosing function's instruction stream.
    auto saved_pending = pending_nested_functions_;
    pending_nested_functions_.clear();
    for (auto& pnf : saved_pending) {
        emit_pending_nested_function(pnf);
    }
}

void CodeGenerator::visit(VarDeclNode& node) {
    // Global variables are handled in the first pass
    // Only process local variables here (inside functions)
    if (current_function_.empty()) {
        return; // Skip global variables in code generation pass
    }

    int elem_size = get_type_size(node.type_name);
    int alloc_size;
    if (node.array_size > 0) {
        alloc_size = elem_size * node.array_size;
    } else {
        alloc_size = elem_size;
    }
    // Align to 8 bytes
    int aligned_size = ((alloc_size + 7) / 8) * 8;
    if (aligned_size < 8) aligned_size = 8;

    stack_offset_ += aligned_size;
    int base_offset = -stack_offset_;
    local_variables_[node.name] = base_offset;
    variable_types_[node.name] = node.type_name;

    // For arrays, store the base address and element size info
    if (node.array_size > 0) {
        array_info_[node.name] = {elem_size, node.array_size};
    }

    if (node.initializer) {
        if (auto* init_list = dynamic_cast<InitializerListNode*>(node.initializer.get())) {
            // Flatten nested InitializerListNodes into a single list
            std::vector<ASTNode*> flat_elements;
            std::function<void(InitializerListNode&)> collect = [&](InitializerListNode& list) {
                for (auto& elem : list.elements) {
                    if (auto* sub = dynamic_cast<InitializerListNode*>(elem.get())) {
                        collect(*sub);
                    } else {
                        flat_elements.push_back(elem.get());
                    }
                }
            };
            collect(*init_list);

            // Emit each flat element to its slot
            int elem_index = 0;
            for (ASTNode* elem : flat_elements) {
                if (auto* desig = dynamic_cast<DesignatedInitNode*>(elem)) {
                    if (!desig->field_name.empty()) {
                        int field_off = get_field_offset(get_struct_name(node.type_name), desig->field_name);
                        if (field_off < 0) field_off = 0;
                        dispatch(desig->value.get());
                        emit("mov %rax, " + std::to_string(base_offset + field_off) + "(%rbp)");
                        continue;
                    } else if (desig->array_index >= 0) {
                        elem_index = desig->array_index;
                    }
                }
                if (elem_index < node.array_size || node.array_size == 0) {
                    dispatch(elem);
                    int slot = base_offset + elem_index * elem_size;
                    if (elem_size == 1) {
                        emit("mov %al, " + std::to_string(slot) + "(%rbp)");
                    } else if (elem_size == 4) {
                        emit("movl %eax, " + std::to_string(slot) + "(%rbp)");
                    } else {
                        emit("mov %rax, " + std::to_string(slot) + "(%rbp)");
                    }
                }
                elem_index++;
            }
        } else {
            // Simple expression initializer
            std::string init_type = infer_expr_type(node.initializer.get());
            bool init_is_float = is_float_type(init_type);
            dispatch(node.initializer.get());
            if (init_is_float) {
                // Float/double initializer: result in %xmm0
                if (is_double_type(node.type_name)) {
                    emit("movsd %xmm0, " + std::to_string(base_offset) + "(%rbp)");
                } else {
                    emit("movss %xmm0, " + std::to_string(base_offset) + "(%rbp)");
                }
                if (!expr_type_stack_.empty()) expr_type_stack_.pop_back();
            } else if (elem_size == 1) {
                emit("mov %al, " + std::to_string(base_offset) + "(%rbp)");
            } else if (elem_size == 4) {
                emit("movl %eax, " + std::to_string(base_offset) + "(%rbp)");
            } else {
                emit("mov %rax, " + std::to_string(base_offset) + "(%rbp)");
            }
            push_expr_type(node.type_name);
        }
    }
}

// Helper to compute member address (address of struct + field offset)
void CodeGenerator::compute_member_address(MemberExprNode& node) {
    // Load base address of struct
    if (auto* id = dynamic_cast<IdentifierExprNode*>(node.object.get())) {
        if (local_variables_.count(id->name)) {
            int offset = local_variables_[id->name];
            emit("lea " + std::to_string(offset) + "(%rbp), %rax");
        } else {
            // Global variable
            emit("lea " + id->name + "(%rip), %rax");
        }
    } else {
        // For other expressions (pointers, etc.)
        dispatch(node.object.get());
        if (node.is_arrow) {
            // Arrow: object is a pointer, dereference it
            emit("mov (%rax), %rax");
        }
    }
    
    // Determine struct type
    std::string struct_name;
    if (auto* id = dynamic_cast<IdentifierExprNode*>(node.object.get())) {
        if (variable_types_.count(id->name)) {
            struct_name = variable_types_[id->name];
            if (struct_name.substr(0, 7) == "struct ") {
                struct_name = struct_name.substr(7);
            }
        }
    }
    
    int offset = 0;
    if (!struct_name.empty()) {
        offset = get_field_offset(struct_name, node.member);
    }
    
    if (offset != 0) {
        emit("lea " + std::to_string(offset) + "(%rax), %rax");
    }
    // If offset is 0, rax already points to the right place
}

void CodeGenerator::visit(StructFieldNode& node) {
    // Struct fields are handled by struct declarations
}

void CodeGenerator::visit(StructDeclNode& node) {
    // Build struct layout for member access
    std::vector<FieldInfo> fields;
    int offset = 0;
    for (auto& field_ast : node.fields) {
        auto* field = static_cast<StructFieldNode*>(field_ast.get());
        int field_size = get_type_size(field->type_name);
        FieldInfo fi;
        fi.name = field->name;
        fi.type = field->type_name;
        fi.offset = offset;
        fi.size = field_size;
        fields.push_back(fi);
        offset += field_size;
    }
    struct_layouts_[node.name] = fields;
}

void CodeGenerator::visit(EnumDeclNode& node) {
    // Enum declarations don't generate code
    // Values are resolved at parse time via enum_constants_ in the parser
}

void CodeGenerator::visit(EnumValueNode& node) {
    // Enum values are handled by EnumDeclNode
}

void CodeGenerator::visit(TypedefDeclNode& node) {
    typedef_map_[node.alias] = node.source_type;
}

void CodeGenerator::visit(SwitchStmtNode& node) {
    std::string end_label = new_label("switch_end");
    std::string default_label = new_label("switch_default");
    bool has_default = false;
    
    // Evaluate condition
    dispatch(node.condition.get());
    emit("push %rax");
    
    // First pass: generate all comparisons as a chain of jumps
    std::vector<std::pair<std::string, ASTPtr>> case_bodies;
    for (auto& case_ast : node.cases) {
        if (case_ast->type == NodeType::CASE_LABEL) {
            auto* case_node = static_cast<CaseLabelNode*>(case_ast.get());
            std::string case_label = new_label("case");
            
            // Compare case value with switch value
            dispatch(case_node->value.get());
            emit("pop %rcx");
            emit("push %rcx");
            emit("cmp %rax, %rcx");
            emit("je " + case_label);
            
            case_bodies.push_back({case_label, std::move(case_node->body)});
        } else if (case_ast->type == NodeType::DEFAULT_LABEL) {
            has_default = true;
            auto* default_node = static_cast<DefaultLabelNode*>(case_ast.get());
            case_bodies.push_back({default_label, std::move(default_node->body)});
        }
    }
    
    // Jump to default or end
    if (has_default) {
        emit("jmp " + default_label);
    } else {
        emit("jmp " + end_label);
    }
    
    emit("pop %rax"); // pop switch value
    
    // Second pass: emit case bodies with labels
    current_switch_end_ = end_label;
    for (auto& [label, body] : case_bodies) {
        emit_label(label);
        if (body) {
            dispatch(body.get());
        }
    }
    
    emit_label(end_label);
}

void CodeGenerator::visit(CaseLabelNode& node) {
    // Case labels are handled by SwitchStmtNode
}

void CodeGenerator::visit(DefaultLabelNode& node) {
    // Default labels are handled by SwitchStmtNode
}

void CodeGenerator::visit(GotoStmtNode& node) {
    emit("jmp " + node.label);
}

void CodeGenerator::visit(LabelStmtNode& node) {
    emit_label(node.label);
    if (node.stmt) {
        dispatch(node.stmt.get());
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
    bool ret_is_float = is_float_type(current_function_return_type_);
    if (node.value) {
        dispatch(node.value.get());
        if (ret_is_float) {
            // The dispatched value should be in %xmm0.  Make sure it is.
            std::string vt = infer_expr_type(node.value.get());
            if (!is_float_type(vt)) {
                // The expression is integer but the function returns float.
                // Convert.
                if (is_double_type(current_function_return_type_)) {
                    emit("cvtsi2sd %rax, %xmm0");
                } else {
                    emit("cvtsi2ss %rax, %xmm0");
                }
            }
        }
    } else {
        if (ret_is_float) {
            // Returning float with no value: zero %xmm0
            if (is_double_type(current_function_return_type_)) {
                emit("xorpd %xmm0, %xmm0");
            } else {
                emit("xorps %xmm0, %xmm0");
            }
        } else {
            emit("mov $0, %rax");
        }
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
    
    std::string saved_loop_start = current_loop_start_;
    std::string saved_loop_end = current_loop_end_;
    current_loop_start_ = start_label;
    current_loop_end_ = end_label;
    
    emit_label(start_label);
    
    dispatch(node.condition.get());
    emit("cmp $0, %rax");
    emit("je " + end_label);
    
    if (node.body) {
        dispatch(node.body.get());
    }
    
    emit("jmp " + start_label);
    emit_label(end_label);
    
    current_loop_start_ = saved_loop_start;
    current_loop_end_ = saved_loop_end;
}

void CodeGenerator::visit(DoWhileStmtNode& node) {
    std::string start_label = new_label("do_while_start");
    std::string end_label = new_label("do_while_end");
    
    std::string saved_loop_start = current_loop_start_;
    std::string saved_loop_end = current_loop_end_;
    current_loop_start_ = start_label;
    current_loop_end_ = end_label;
    
    emit_label(start_label);
    
    if (node.body) {
        dispatch(node.body.get());
    }
    
    dispatch(node.condition.get());
    emit("cmp $0, %rax");
    emit("jne " + start_label);
    
    emit_label(end_label);
    
    current_loop_start_ = saved_loop_start;
    current_loop_end_ = saved_loop_end;
}

void CodeGenerator::visit(ForStmtNode& node) {
    std::string start_label = new_label("for_start");
    std::string cond_label = new_label("for_cond");
    std::string end_label = new_label("for_end");
    
    std::string saved_loop_start = current_loop_start_;
    std::string saved_loop_end = current_loop_end_;
    current_loop_start_ = start_label;
    current_loop_end_ = end_label;
    
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
    
    current_loop_start_ = saved_loop_start;
    current_loop_end_ = saved_loop_end;
}

void CodeGenerator::visit(BreakStmtNode& node) {
    if (!current_switch_end_.empty()) {
        emit("jmp " + current_switch_end_);
    } else if (!current_loop_end_.empty()) {
        emit("jmp " + current_loop_end_);
    }
}

void CodeGenerator::visit(ContinueStmtNode& node) {
    if (!current_loop_start_.empty()) {
        emit("jmp " + current_loop_start_);
    }
}

void CodeGenerator::visit(BinaryExprNode& node) {
    generate_binary(node);
}

void CodeGenerator::visit(UnaryExprNode& node) {
    generate_unary(node);
}

void CodeGenerator::visit(AssignExprNode& node) {
    // Handle assignment to member expression
    if (auto* member = dynamic_cast<MemberExprNode*>(node.target.get())) {
        // Evaluate the value first
        dispatch(node.value.get());
        emit("push %rax");
        
        // Compute address of the member
        compute_member_address(*member);
        
        // Pop value and store
        emit("pop %rcx");
        emit("mov %rcx, (%rax)");
        return;
    }
    
    // Handle assignment to array index
    if (auto* idx = dynamic_cast<IndexExprNode*>(node.target.get())) {
        // Evaluate the value first
        dispatch(node.value.get());
        emit("push %rax");
        
        // Compute address of the element
        std::string base_type;
        int elem_size = 4;
        if (auto* id = dynamic_cast<IdentifierExprNode*>(idx->array.get())) {
            if (array_info_.count(id->name)) {
                elem_size = array_info_[id->name].elem_size;
            }
        }
        
        dispatch(idx->array.get());
        emit("push %rax");
        dispatch(idx->index.get());
        if (elem_size > 1) {
            emit("imul $" + std::to_string(elem_size) + ", %rax");
        }
        emit("pop %rcx");
        emit("add %rcx, %rax");
        
        // Pop value and store
        emit("pop %rcx");
        emit("mov %rcx, (%rax)");
        return;
    }
    
    std::string val_type = infer_expr_type(node.value.get());
    bool val_is_float = is_float_type(val_type);
    dispatch(node.value.get());

    if (auto* id = dynamic_cast<IdentifierExprNode*>(node.target.get())) {
        if (local_variables_.count(id->name)) {
            int offset = local_variables_[id->name];
            int sz = 4;
            std::string tgt_type = "int";
            if (variable_types_.count(id->name)) {
                tgt_type = variable_types_[id->name];
                sz = get_type_size(tgt_type);
            }
            if (val_is_float && is_float_type(tgt_type)) {
                if (is_double_type(tgt_type)) {
                    emit("movsd %xmm0, " + std::to_string(offset) + "(%rbp)");
                } else {
                    emit("movss %xmm0, " + std::to_string(offset) + "(%rbp)");
                }
                if (!expr_type_stack_.empty()) expr_type_stack_.pop_back();
            } else if (sz == 1) emit("mov %al, " + std::to_string(offset) + "(%rbp)");
            else if (sz == 2) emit("mov %ax, " + std::to_string(offset) + "(%rbp)");
            else if (sz == 4) emit("movl %eax, " + std::to_string(offset) + "(%rbp)");
            else emit("mov %rax, " + std::to_string(offset) + "(%rbp)");
        } else {
            int cap = find_capture_offset(id->name);
            if (cap >= 0) {
                int sz = 4;
                for (const auto& c : current_captures_) {
                    if (c.name == id->name) { sz = c.size; break; }
                }
                emit("mov %rax, %rcx");
                emit("mov " + std::to_string(current_ctx_stack_offset_) + "(%rbp), %rdx");
                if (sz == 1) emit("mov %cl, " + std::to_string(cap) + "(%rdx)");
                else if (sz == 2) emit("mov %cx, " + std::to_string(cap) + "(%rdx)");
                else if (sz == 4) emit("movl %ecx, " + std::to_string(cap) + "(%rdx)");
                else emit("mov %rcx, " + std::to_string(cap) + "(%rdx)");
                return;
            }
            // Check if it's a global variable
            for (const auto& gvar : global_variables_) {
                if (gvar.name == id->name) {
                    int sz = get_type_size(gvar.type);
                    if (sz == 1) emit("mov %al, " + id->name + "(%rip)");
                    else if (sz == 4) emit("movl %eax, " + id->name + "(%rip)");
                    else emit("mov %rax, " + id->name + "(%rip)");
                    return;
                }
            }
        }
    }
}

void CodeGenerator::visit(CompoundAssignExprNode& node) {
    // Handle compound assignment to member expression
    if (auto* member = dynamic_cast<MemberExprNode*>(node.target.get())) {
        // Compute address of member
        compute_member_address(*member);
        emit("push %rax"); // save address
        
        // Load current value
        emit("mov (%rax), %rax");
        emit("push %rax"); // save current value
        
        // Evaluate right operand
        dispatch(node.value.get());
        
        // Apply operator
        emit("mov %rax, %rcx");
        emit("pop %rax"); // restore current value
        
        switch (node.op) {
            case OpKind::ADD: emit("add %rcx, %rax"); break;
            case OpKind::SUB: emit("sub %rcx, %rax"); break;
            case OpKind::MUL: emit("imul %rcx, %rax"); break;
            case OpKind::DIV: emit("cqo"); emit("idiv %rcx"); break;
            case OpKind::MOD: emit("cqo"); emit("idiv %rcx"); emit("mov %rdx, %rax"); break;
            default: break;
        }
        
        // Store result back
        emit("pop %rcx"); // restore address
        emit("mov %rax, (%rcx)");
        return;
    }
    
    // Load current value of target
    auto* id = dynamic_cast<IdentifierExprNode*>(node.target.get());
    if (!id) return;

    bool is_local = local_variables_.count(id->name) > 0;
    bool is_capture = !is_local && find_capture_offset(id->name) >= 0;
    if (!is_local && !is_capture) return;

    int offset = 0;
    int cap = -1;
    if (is_local) {
        offset = local_variables_[id->name];
    } else {
        cap = find_capture_offset(id->name);
    }

    std::string var_type = "int";
    if (variable_types_.count(id->name)) {
        var_type = variable_types_[id->name];
    }
    bool target_is_float = is_float_type(var_type);
    bool target_is_double = is_double_type(var_type);

    // Load current value (float → xmm0 via movss/movsd; int → rax via mov)
    if (is_local) {
        if (target_is_float) {
            if (target_is_double) {
                emit("movsd " + std::to_string(offset) + "(%rbp), %xmm0");
            } else {
                emit("movss " + std::to_string(offset) + "(%rbp), %xmm0");
            }
        } else {
            int sz = get_type_size(var_type);
            if (sz == 1) emit("movzbl " + std::to_string(offset) + "(%rbp), %eax");
            else if (sz == 2) emit("movzwl " + std::to_string(offset) + "(%rbp), %eax");
            else if (sz == 4) emit("movl " + std::to_string(offset) + "(%rbp), %eax");
            else emit("mov " + std::to_string(offset) + "(%rbp), %rax");
        }
    } else {
        int sz = 4;
        for (const auto& c : current_captures_) {
            if (c.name == id->name) { sz = c.size; break; }
        }
        emit("mov " + std::to_string(current_ctx_stack_offset_) + "(%rbp), %rdx");
        if (sz == 1) emit("movzbl " + std::to_string(cap) + "(%rdx), %eax");
        else if (sz == 2) emit("movzwl " + std::to_string(cap) + "(%rdx), %eax");
        else if (sz == 4) emit("movl " + std::to_string(cap) + "(%rdx), %eax");
        else emit("mov " + std::to_string(cap) + "(%rdx), %rax");
    }

    // Save left side (target's current value) for the operation
    if (target_is_float) {
        emit("sub $16, %rsp");
        emit(target_is_double ? "movsd %xmm0, (%rsp)" : "movss %xmm0, (%rsp)");
    } else {
        emit("push %rax");
    }

    // Evaluate right operand
    dispatch(node.value.get());
    if (!expr_type_stack_.empty()) expr_type_stack_.pop_back();

    if (target_is_float) {
        // Right operand is now in %rax (if int) or %xmm0 (if float).
        // Convert int → float/double if needed.
        std::string val_type = infer_expr_type(node.value.get());
        if (!is_float_type(val_type)) {
            if (target_is_double) {
                emit("cvtsi2sd %rax, %xmm0");
            } else {
                emit("cvtsi2ss %rax, %xmm0");
            }
        }
        // Reload left from stack into %xmm1
        emit(target_is_double ? "movsd (%rsp), %xmm1" : "movss (%rsp), %xmm1");
        emit("add $16, %rsp");

        const char* sfx = target_is_double ? "sd" : "ss";
        switch (node.op) {
            case OpKind::ADD: emit(std::string("add") + sfx + " %xmm1, %xmm0"); break;
            case OpKind::SUB: emit(std::string("sub") + sfx + " %xmm1, %xmm0"); break;
            case OpKind::MUL: emit(std::string("mul") + sfx + " %xmm1, %xmm0"); break;
            case OpKind::DIV: emit(std::string("div") + sfx + " %xmm1, %xmm0"); break;
            case OpKind::MOD:
                error_message_ = "MOD not supported for float";
                break;
            default:
                error_message_ = "Operator not supported for float compound assign";
                break;
        }
    } else {
        // Integer path
        emit("mov %rax, %rcx");
        emit("pop %rax");
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
    }

    // Store result back
    if (is_local) {
        if (target_is_float) {
            if (target_is_double) {
                emit("movsd %xmm0, " + std::to_string(offset) + "(%rbp)");
            } else {
                emit("movss %xmm0, " + std::to_string(offset) + "(%rbp)");
            }
        } else {
            emit("mov %rax, " + std::to_string(offset) + "(%rbp)");
        }
    } else {
        int sz = get_type_size(var_type);
        emit("mov %rax, %rcx");
        emit("mov " + std::to_string(current_ctx_stack_offset_) + "(%rbp), %rdx");
        if (sz == 1) emit("mov %cl, " + std::to_string(cap) + "(%rdx)");
        else if (sz == 2) emit("mov %cx, " + std::to_string(cap) + "(%rdx)");
        else if (sz == 4) emit("movl %ecx, " + std::to_string(cap) + "(%rdx)");
        else emit("mov %rcx, " + std::to_string(cap) + "(%rdx)");
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
    if (node.is_type) {
        if (node.type_name == "int" || node.type_name == "const int") {
            emit("mov $4, %rax");
        } else if (node.type_name == "char" || node.type_name == "const char") {
            emit("mov $1, %rax");
        } else if (node.type_name == "bool") {
            emit("mov $1, %rax");
        } else if (node.type_name == "void" || node.type_name == "const void") {
            emit("mov $1, %rax"); // sizeof(void) is 1 in GCC
        } else if (node.type_name == "float" || node.type_name == "const float") {
            emit("mov $4, %rax");
        } else if (node.type_name == "double" || node.type_name == "const double") {
            emit("mov $8, %rax");
        } else if (node.type_name.find('*') != std::string::npos) {
            emit("mov $8, %rax"); // pointer
        } else if (node.type_name.substr(0, 7) == "struct ") {
            std::string sn = node.type_name.substr(7);
            emit("mov $" + std::to_string(get_struct_size(sn)) + ", %rax");
        } else {
            emit("mov $8, %rax"); // default
        }
    } else {
        // sizeof expression — if it's an identifier that is an array,
        // emit the array's total size; otherwise emit pointer size (simplified).
        bool handled = false;
        if (auto* id = dynamic_cast<IdentifierExprNode*>(node.expr.get())) {
            if (array_info_.count(id->name)) {
                int elem_sz = array_info_[id->name].elem_size;
                int len = array_info_[id->name].length;
                emit("mov $" + std::to_string(elem_sz * len) + ", %rax");
                handled = true;
            }
        }
        if (!handled) {
            dispatch(node.expr.get());
            emit("mov $8, %rax"); // simplified
        }
    }
}

void CodeGenerator::visit(CastExprNode& node) {
    // Type cast. We only need to emit conversion instructions when the
    // source and target are different categories (int ↔ float). For
    // same-category casts (e.g. int→long), the existing code is fine.
    std::string src = infer_expr_type(node.expr.get());
    std::string dst = node.target_type;
    bool src_float = is_float_type(src);
    bool dst_float = is_float_type(dst);

    if (src_float && !dst_float) {
        // Float/double → integer: cvttss2si / cvttsd2si (truncation)
        dispatch(node.expr.get());
        std::string actual_src = pop_expr_type();
        if (is_double_type(actual_src)) {
            emit("cvttsd2si %xmm0, %rax");
        } else {
            emit("cvttss2si %xmm0, %rax");
        }
        push_expr_type(dst);
        return;
    }
    if (!src_float && dst_float) {
        // Integer → float/double: cvtsi2ss / cvtsi2sd
        dispatch(node.expr.get());
        std::string actual_src = pop_expr_type();
        if (is_double_type(dst)) {
            emit("cvtsi2sd %rax, %xmm0");
        } else {
            emit("cvtsi2ss %rax, %xmm0");
        }
        push_expr_type(dst);
        return;
    }
    if (src_float && dst_float) {
        // Float ↔ double: cvtss2sd / cvtsd2ss
        dispatch(node.expr.get());
        std::string actual_src = pop_expr_type();
        if (is_double_type(actual_src) && !is_double_type(dst)) {
            emit("cvtsd2ss %xmm0, %xmm0");
        } else if (!is_double_type(actual_src) && is_double_type(dst)) {
            emit("cvtss2sd %xmm0, %xmm0");
        }
        push_expr_type(dst);
        return;
    }
    // Integer to integer (or pointer): no-op
    dispatch(node.expr.get());
    if (!expr_type_stack_.empty()) {
        expr_type_stack_.pop_back();
    }
    push_expr_type(dst);
}

void CodeGenerator::visit(CallExprNode& node) {
    static const char* param_regs[] = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};
    
    // Handle __builtin_expect: just evaluate first argument
    if (node.function_name == "__builtin_expect") {
        if (!node.arguments.empty()) {
            dispatch(node.arguments[0].get());
        }
        return;
    }
    
    // Handle __builtin_offsetof(type, member): return offset
    if (node.function_name == "__builtin_offsetof" && node.arguments.size() == 2) {
        // Extract type name and member name from arguments
        if (auto* type_arg = dynamic_cast<IdentifierExprNode*>(node.arguments[0].get())) {
            if (auto* member_arg = dynamic_cast<IdentifierExprNode*>(node.arguments[1].get())) {
                std::string type_name = type_arg->name;
                std::string member_name = member_arg->name;
                // Strip struct/union prefix
                if (type_name.substr(0, 7) == "struct ") type_name = type_name.substr(7);
                else if (type_name.substr(0, 6) == "union ") type_name = type_name.substr(6);
                int offset = get_field_offset(type_name, member_name);
                emit("mov $" + std::to_string(offset >= 0 ? offset : 0) + ", %rax");
                return;
            }
        }
        emit("mov $0, %rax");
        return;
    }
    
    // Handle __builtin_popcount: inline popcount using popcnt instruction
    if (node.function_name == "__builtin_popcount") {
        if (!node.arguments.empty()) {
            dispatch(node.arguments[0].get());
            emit("popcnt %eax, %eax");
        }
        return;
    }

    // Nested function call: pass a context struct as the first argument.
    auto nit = nested_func_info_.find(node.function_name);
    if (nit != nested_func_info_.end()) {
        const NestedFuncInfo& info = nit->second;
        int ctx_size = info.ctx_size;

        // Allocate space for the context struct on the stack.
        emit("sub $" + std::to_string(ctx_size) + ", %rsp");

        // Populate the context struct with current values of the captures.
        for (const auto& cap : info.captures) {
            auto lit = local_variables_.find(cap.name);
            int parent_off = (lit != local_variables_.end()) ? lit->second : cap.parent_offset;
            int sz = cap.size;
            if (sz == 1) emit("movzbl " + std::to_string(parent_off) + "(%rbp), %eax");
            else if (sz == 2) emit("movzwl " + std::to_string(parent_off) + "(%rbp), %eax");
            else if (sz == 4) emit("movl " + std::to_string(parent_off) + "(%rbp), %eax");
            else emit("mov " + std::to_string(parent_off) + "(%rbp), %rax");
            if (sz == 1) emit("mov %al, " + std::to_string(cap.ctx_offset) + "(%rsp)");
            else if (sz == 2) emit("mov %ax, " + std::to_string(cap.ctx_offset) + "(%rsp)");
            else if (sz == 4) emit("movl %eax, " + std::to_string(cap.ctx_offset) + "(%rsp)");
            else emit("mov %rax, " + std::to_string(cap.ctx_offset) + "(%rsp)");
        }

        // Evaluate user arguments and push them onto the stack
        // (right to left) so they sit above the context struct.
        int num_user_args = std::min((int)node.arguments.size(), 5);
        for (int i = num_user_args - 1; i >= 0; i--) {
            dispatch(node.arguments[i].get());
            emit("push %rax");
        }

        // Pop user arguments into %rsi, %rdx, %rcx, %r8, %r9
        static const char* user_param_regs[] = {"%rsi", "%rdx", "%rcx", "%r8", "%r9"};
        for (int i = 0; i < num_user_args; i++) {
            emit("pop " + std::string(user_param_regs[i]));
        }

        // Set %rdi to point at the context struct
        emit("mov %rsp, %rdi");

        emit("call " + node.function_name);

        // Deallocate the context struct
        emit("add $" + std::to_string(ctx_size) + ", %rsp");
        return;
    }

    // Evaluate arguments.  System V x86-64 ABI: integer/pointer args go in
    // %rdi, %rsi, %rdx, %rcx, %r8, %r9 (in order); float/double args go in
    // %xmm0..%xmm7 (in order).  Mixing is allowed: e.g. for `foo(int, double,
    // int)`, a goes in %rdi, b in %xmm0, c in %rsi.
    //
    // Float args are processed right-to-left so that earlier (lower-index)
    // args are placed in lower-numbered XMM registers after later args have
    // been moved out of %xmm0.  Integer args are pushed onto the stack in
    // left-to-right order and popped into the appropriate integer register
    // in reverse, so the net effect is that arg N (in %rN) sits at offset
    // int_idx-1 in the popped sequence.
    int num_args = (int)node.arguments.size();
    int int_total = 0;
    int float_total = 0;
    for (int i = 0; i < num_args; i++) {
        std::string at = infer_expr_type(node.arguments[i].get());
        if (is_float_type(at)) float_total++;
        else int_total++;
    }
    int int_count = int_total;
    int float_count = float_total;

    for (int i = num_args - 1; i >= 0; i--) {
        dispatch(node.arguments[i].get());
        std::string actual = pop_expr_type();
        if (is_float_type(actual)) {
            int idx = --float_count;
            if (idx < 8) {
                std::string xreg = "%xmm" + std::to_string(idx);
                std::string mop = is_double_type(actual) ? "movsd" : "movss";
                if (xreg != "%xmm0") {
                    emit(mop + " %xmm0, " + xreg);
                }
            } else {
                emit("sub $16, %rsp");
                emit("movsd %xmm0, (%rsp)");
            }
        } else {
            int idx = --int_count;
            if (idx < 6) {
                emit("push %rax");
            } else {
                emit("push %rax");
            }
        }
    }

    // Pop integer args in stack order to %rdi..%r9 (left-to-right).  We
    // processed and pushed args right-to-left, so the stack's top is the
    // first (lowest-index) int arg, and pops in order yield args 0, 1, 2...
    int int_pushes = std::min(int_total, 6);
    for (int i = 0; i < int_pushes; i++) {
        emit("pop " + std::string(param_regs[i]));
    }

    // Check if this is an indirect call (function pointer variable)
    bool is_indirect = false;
    if (variable_types_.count(node.function_name)) {
        std::string vtype = variable_types_[node.function_name];
        if (vtype.find("(*)") != std::string::npos) {
            is_indirect = true;
        }
    }

    if (is_indirect) {
        // Load function pointer from variable and call indirectly
        if (local_variables_.count(node.function_name)) {
            int offset = local_variables_[node.function_name];
            emit("mov " + std::to_string(offset) + "(%rbp), %rax");
        } else {
            emit("lea " + node.function_name + "(%rip), %rax");
            emit("mov (%rax), %rax");
        }
        emit("call *%rax");
    } else {
        emit("call " + node.function_name);
    }
}

void CodeGenerator::visit(IndexExprNode& node) {
    // Check if this is array indexing or pointer indexing
    std::string base_type;
    int elem_size = 4; // default to int

    if (auto* id = dynamic_cast<IdentifierExprNode*>(node.array.get())) {
        if (array_info_.count(id->name)) {
            elem_size = array_info_[id->name].elem_size;
        } else if (variable_types_.count(id->name)) {
            // For pointer types, use the pointed-to type's size, not the pointer's size
            std::string vtype = variable_types_[id->name];
            if (vtype.find('*') != std::string::npos) {
                // Extract pointed-to type
                std::string pointee = vtype;
                size_t p = pointee.find('*');
                while (p != std::string::npos) {
                    pointee.erase(p, 1);
                    p = pointee.find('*');
                }
                // Trim whitespace
                size_t s = pointee.find_first_not_of(" \t");
                size_t e = pointee.find_last_not_of(" \t");
                if (s != std::string::npos) pointee = pointee.substr(s, e - s + 1);
                elem_size = get_type_size(pointee);
            } else {
                elem_size = get_type_size(vtype);
            }
        }
    }
    
    // Load base address (for arrays, this is the address of the first element)
    dispatch(node.array.get());
    emit("push %rax");
    
    // Load index
    dispatch(node.index.get());
    
    // Multiply index by element size
    if (elem_size > 1) {
        emit("imul $" + std::to_string(elem_size) + ", %rax");
    }
    
    // Compute address: base + index * element_size
    emit("pop %rcx");
    emit("add %rcx, %rax");
    
    // Load value at address
    if (elem_size == 1) {
        emit("movzbl (%rax), %eax");
    } else if (elem_size == 4) {
        emit("movl (%rax), %eax");
    } else if (elem_size == 8) {
        emit("mov (%rax), %rax");
    } else if (elem_size == 2) {
        emit("movzwl (%rax), %eax");
    } else {
        emit("mov (%rax), %rax");
    }
}

void CodeGenerator::visit(MemberExprNode& node) {
    // Compute address of the member
    compute_member_address(node);
    
    // Load value at that address
    emit("mov (%rax), %rax");
}

void CodeGenerator::visit(DerefExprNode& node) {
    dispatch(node.operand.get());
    // Determine load size based on the operand's type (cast or pointer)
    int load_size = 8;
    if (auto* cast = dynamic_cast<CastExprNode*>(node.operand.get())) {
        load_size = get_type_size(cast->target_type);
        if (cast->target_type.find('*') != std::string::npos) {
            // Pointer deref: load 8 bytes (the pointer's value)
            load_size = 8;
        }
    } else if (auto* id = dynamic_cast<IdentifierExprNode*>(node.operand.get())) {
        if (variable_types_.count(id->name)) {
            std::string vt = variable_types_[id->name];
            if (vt.find('*') != std::string::npos) {
                // Pointer variable: load the pointed-to type's size
                std::string pointee = vt;
                size_t p = pointee.find('*');
                while (p != std::string::npos) { pointee.erase(p, 1); p = pointee.find('*'); }
                size_t s = pointee.find_first_not_of(" \t");
                size_t e = pointee.find_last_not_of(" \t");
                if (s != std::string::npos) pointee = pointee.substr(s, e - s + 1);
                load_size = get_type_size(pointee);
            }
        }
    }
    if (load_size == 1) emit("movzbl (%rax), %eax");
    else if (load_size == 2) emit("movzwl (%rax), %eax");
    else if (load_size == 4) emit("movl (%rax), %eax");
    else emit("mov (%rax), %rax");
}

void CodeGenerator::visit(AddressOfExprNode& node) {
    if (auto* id = dynamic_cast<IdentifierExprNode*>(node.operand.get())) {
        if (local_variables_.count(id->name)) {
            int offset = local_variables_[id->name];
            emit("lea " + std::to_string(offset) + "(%rbp), %rax");
        } else {
            int cap = find_capture_offset(id->name);
            if (cap >= 0) {
                emit("mov " + std::to_string(current_ctx_stack_offset_) + "(%rbp), %rax");
                if (cap != 0) {
                    emit("add $" + std::to_string(cap) + ", %rax");
                }
            }
        }
    }
}

void CodeGenerator::visit(StmtExprNode& node) {
    if (node.body) {
        auto& block = static_cast<BlockNode&>(*node.body);
        for (auto& stmt : block.statements) {
            dispatch(stmt.get());
        }
    }
}

void CodeGenerator::visit(AsmStmtNode& node) {
    emit("# asm: " + node.assembly);
    // Emit the raw assembly string
    output_ << "    " << node.assembly << "\n";
}

void CodeGenerator::visit(IntegerLiteralNode& node) {
    // Use `movabsq` for values that don't fit in a signed 32-bit immediate
    // (the assembler sign-extends 32-bit `mov` immediates to 64 bits).
    long long v = node.value;
    if (v >= -2147483648LL && v <= 2147483647LL) {
        emit("mov $" + std::to_string(v) + ", %rax");
    } else {
        emit("movabsq $" + std::to_string(v) + ", %rax");
    }
}

void CodeGenerator::visit(FloatLiteralNode& node) {
    // Load the IEEE 754 bit pattern into an XMM register via a bit-pattern
    // transfer.  For float: zero-extend the 32-bit pattern from %eax to %xmm0
    // (the upper 32 bits of %xmm0 are zeroed).  For double: move a 64-bit
    // immediate from %rax into %xmm0.
    double d = node.value;
    bool is_single = node.is_single_precision;
    if (is_single) {
        float f = static_cast<float>(d);
        uint32_t float_bits = 0;
        std::memcpy(&float_bits, &f, sizeof(float_bits));
        int32_t signed_bits;
        std::memcpy(&signed_bits, &float_bits, sizeof(signed_bits));
        emit("mov $" + std::to_string(signed_bits) + ", %eax");
        emit("movd %eax, %xmm0");
        push_expr_type("float");
    } else {
        uint64_t double_bits = 0;
        std::memcpy(&double_bits, &d, sizeof(double_bits));
        emit("movabsq $" + std::to_string(double_bits) + ", %rax");
        emit("movq %rax, %xmm0");
        push_expr_type("double");
    }
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
    std::string var_type = "int";
    if (variable_types_.count(node.name)) {
        var_type = variable_types_[node.name];
    }

    if (local_variables_.count(node.name)) {
        int offset = local_variables_[node.name];
        // For arrays, return the address (base of the array)
        if (array_info_.count(node.name)) {
            emit("lea " + std::to_string(offset) + "(%rbp), %rax");
            push_expr_type(var_type);
            return;
        }
        // Use the variable's size for the load
        int sz = 8;
        if (variable_types_.count(node.name)) {
            sz = get_type_size(variable_types_[node.name]);
        }
        if (is_float_type(var_type)) {
            if (is_double_type(var_type)) {
                emit("movsd " + std::to_string(offset) + "(%rbp), %xmm0");
            } else {
                emit("movss " + std::to_string(offset) + "(%rbp), %xmm0");
            }
        } else if (sz == 1) {
            emit("movzbl " + std::to_string(offset) + "(%rbp), %eax");
        } else if (sz == 2) {
            emit("movzwl " + std::to_string(offset) + "(%rbp), %eax");
        } else if (sz == 4) {
            emit("movl " + std::to_string(offset) + "(%rbp), %eax");
        } else {
            emit("mov " + std::to_string(offset) + "(%rbp), %rax");
        }
        push_expr_type(var_type);
        return;
    }
    // Check if it's a captured variable from the enclosing function
    int cap = find_capture_offset(node.name);
    if (cap >= 0) {
        int sz = 4;
        for (const auto& c : current_captures_) {
            if (c.name == node.name) { sz = c.size; break; }
        }
        // Load __ctx into a scratch register, then load the captured value
        emit("mov " + std::to_string(current_ctx_stack_offset_) + "(%rbp), %rdx");
        if (sz == 1) emit("movzbl " + std::to_string(cap) + "(%rdx), %eax");
        else if (sz == 2) emit("movzwl " + std::to_string(cap) + "(%rdx), %eax");
        else if (sz == 4) emit("movl " + std::to_string(cap) + "(%rdx), %eax");
        else emit("mov " + std::to_string(cap) + "(%rdx), %rax");
        push_expr_type(var_type);
        return;
    }
    // Check if it's a global variable
    for (const auto& gvar : global_variables_) {
        if (gvar.name == node.name) {
            if (array_info_.count(node.name)) {
                emit("lea " + node.name + "(%rip), %rax");
            } else {
                emit("mov " + node.name + "(%rip), %rax");
            }
            push_expr_type(var_type);
            return;
        }
    }
    // Check if it's a function
    emit("lea " + node.name + "(%rip), %rax");
    push_expr_type(var_type);
}

// Helper methods

void CodeGenerator::generate_expression(ASTNode* node) {
    if (node) {
        dispatch(node);
    }
}

void CodeGenerator::generate_binary(BinaryExprNode& node) {
    // Determine static result type.  Float/double operands use SSE; everything
    // else uses the integer ABI (result in %rax, second operand pushed onto
    // the stack during evaluation).
    std::string ltype = infer_expr_type(node.left.get());
    std::string rtype = infer_expr_type(node.right.get());
    bool result_is_double = is_double_type(ltype) || is_double_type(rtype);
    bool result_is_float = is_float_type(ltype) || is_float_type(rtype);

    if (result_is_float) {
        const char* movop = result_is_double ? "movsd" : "movss";
        const char* cvtop = result_is_double ? "cvtsi2sd" : "cvtsi2ss";
        const char* suffix = result_is_double ? "sd" : "ss";
        int slot_size = result_is_double ? 8 : 4;

        // Evaluate right operand: result in %xmm0 (float) or %rax (int).
        dispatch(node.right.get());
        std::string actual_rtype = pop_expr_type();
        if (!is_float_type(actual_rtype)) {
            emit(std::string(cvtop) + " %rax, %xmm0");
        }
        // Spill right operand to a stack slot so the left operand's
        // evaluation (which may itself be a nested float expression that
        // clobbers %xmm0/%xmm1) does not destroy it.
        emit("sub $16, %rsp");
        emit(std::string(movop) + " %xmm0, (%rsp)");

        // Evaluate left operand.
        dispatch(node.left.get());
        std::string actual_ltype = pop_expr_type();
        if (!is_float_type(actual_ltype)) {
            emit(std::string(cvtop) + " %rax, %xmm0");
        }
        // Move spilled right into %xmm1.
        emit(std::string(movop) + " (%rsp), %xmm1");
        emit("add $16, %rsp");

        switch (node.op) {
            case OpKind::ADD:
                emit(std::string("add") + suffix + " %xmm1, %xmm0");
                break;
            case OpKind::SUB:
                emit(std::string("sub") + suffix + " %xmm1, %xmm0");
                break;
            case OpKind::MUL:
                emit(std::string("mul") + suffix + " %xmm1, %xmm0");
                break;
            case OpKind::DIV:
                emit(std::string("div") + suffix + " %xmm1, %xmm0");
                break;
            case OpKind::MOD:
                error_message_ = "MOD not supported for floating point";
                emit("xorpd %xmm0, %xmm0");
                break;
            case OpKind::EQ:
            case OpKind::NE:
            case OpKind::LT:
            case OpKind::GT:
            case OpKind::LE:
            case OpKind::GE: {
                emit(std::string("ucomi") + suffix + " %xmm1, %xmm0");
                switch (node.op) {
                    case OpKind::EQ: emit("sete %al"); break;
                    case OpKind::NE: emit("setne %al"); break;
                    case OpKind::LT: emit("setb %al"); break;
                    case OpKind::LE: emit("setbe %al"); break;
                    case OpKind::GT: emit("seta %al"); break;
                    case OpKind::GE: emit("setae %al"); break;
                    default: break;
                }
                emit("movzbq %al, %rax");
                break;
            }
            case OpKind::AND: {
                // Logical AND on floats: result is 1 if both operands are non-zero.
                std::string fl = new_label("and_false");
                std::string el = new_label("and_end");
                if (result_is_double) {
                    emit("xorpd %xmm2, %xmm2");
                    emit("ucomisd %xmm2, %xmm1");
                } else {
                    emit("xorps %xmm2, %xmm2");
                    emit("ucomiss %xmm2, %xmm1");
                }
                emit("je " + fl);
                if (result_is_double) {
                    emit("ucomisd %xmm2, %xmm0");
                } else {
                    emit("ucomiss %xmm2, %xmm0");
                }
                emit("je " + fl);
                emit("mov $1, %rax");
                emit("jmp " + el);
                emit_label(fl);
                emit("mov $0, %rax");
                emit_label(el);
                break;
            }
            case OpKind::OR: {
                // Logical OR on floats: result is 1 if either operand is non-zero.
                std::string tl = new_label("or_true");
                std::string el = new_label("or_end");
                if (result_is_double) {
                    emit("xorpd %xmm2, %xmm2");
                    emit("ucomisd %xmm2, %xmm0");
                } else {
                    emit("xorps %xmm2, %xmm2");
                    emit("ucomiss %xmm2, %xmm0");
                }
                emit("jne " + tl);
                if (result_is_double) {
                    emit("ucomisd %xmm2, %xmm1");
                } else {
                    emit("ucomiss %xmm2, %xmm1");
                }
                emit("jne " + tl);
                emit("mov $0, %rax");
                emit("jmp " + el);
                emit_label(tl);
                emit("mov $1, %rax");
                emit_label(el);
                break;
            }
            case OpKind::BIT_AND:
            case OpKind::BIT_OR:
            case OpKind::BIT_XOR:
            case OpKind::LSHIFT:
            case OpKind::RSHIFT:
                error_message_ = "Bitwise operators not supported for floating point";
                emit("xorpd %xmm0, %xmm0");
                break;
            default:
                error_message_ = "Unsupported binary operator for float";
                break;
        }
        push_expr_type(result_is_double ? "double" : "float");
        return;
    }

    // Integer path
    dispatch(node.right.get());
    emit("push %rax");

    dispatch(node.left.get());

    emit("pop %rcx");

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
    push_expr_type(ltype.empty() ? "int" : ltype);
}

void CodeGenerator::generate_unary(UnaryExprNode& node) {
    std::string operand_type = infer_expr_type(node.operand.get());
    bool operand_is_float = is_float_type(operand_type);
    dispatch(node.operand.get());

    switch (node.op) {
        case OpKind::UMINUS:
            if (operand_is_float) {
                std::string actual = pop_expr_type();
                if (is_double_type(actual)) {
                    // -0.0 - x  (subtraction from a -0.0 pattern)
                    // Or use a sign-bit flip via an int constant in %xmm.
                    emit("movabsq $0x8000000000000000, %rax");
                    emit("movq %rax, %xmm1");
                    emit("xorpd %xmm1, %xmm0");
                } else {
                    emit("mov $0x80000000, %eax");
                    emit("movd %eax, %xmm1");
                    emit("xorps %xmm1, %xmm0");
                }
                push_expr_type(actual);
            } else {
                emit("neg %rax");
            }
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
        case OpKind::DEREF: {
            // Determine load size based on the operand's type
            int load_size = 8;
            if (auto* id = dynamic_cast<IdentifierExprNode*>(node.operand.get())) {
                if (variable_types_.count(id->name)) {
                    std::string vt = variable_types_[id->name];
                    if (vt.find('*') != std::string::npos) {
                        std::string pointee = vt;
                        size_t p = pointee.find('*');
                        while (p != std::string::npos) { pointee.erase(p, 1); p = pointee.find('*'); }
                        size_t s = pointee.find_first_not_of(" \t");
                        size_t e = pointee.find_last_not_of(" \t");
                        if (s != std::string::npos) pointee = pointee.substr(s, e - s + 1);
                        load_size = get_type_size(pointee);
                    }
                }
            } else if (auto* cast = dynamic_cast<CastExprNode*>(node.operand.get())) {
                std::string tt = cast->target_type;
                if (tt.find('*') != std::string::npos) {
                    std::string pointee = tt;
                    size_t p = pointee.find('*');
                    while (p != std::string::npos) { pointee.erase(p, 1); p = pointee.find('*'); }
                    size_t s = pointee.find_first_not_of(" \t");
                    size_t e = pointee.find_last_not_of(" \t");
                    if (s != std::string::npos) pointee = pointee.substr(s, e - s + 1);
                    load_size = get_type_size(pointee);
                }
            }
            if (load_size == 1) emit("movzbl (%rax), %eax");
            else if (load_size == 2) emit("movzwl (%rax), %eax");
            else if (load_size == 4) emit("movl (%rax), %eax");
            else emit("mov (%rax), %rax");
            break;
        }
        case OpKind::ADDRESS_OF:
            // Address-of: rax should already contain the address from dispatch
            // Actually, we need to get the address of the operand
            // The operand should be an lvalue (identifier, member, deref, etc.)
            if (auto* id = dynamic_cast<IdentifierExprNode*>(node.operand.get())) {
                if (local_variables_.count(id->name)) {
                    int offset = local_variables_[id->name];
                    emit("lea " + std::to_string(offset) + "(%rbp), %rax");
                } else {
                    emit("lea " + id->name + "(%rip), %rax");
                }
            } else if (auto* deref = dynamic_cast<DerefExprNode*>(node.operand.get())) {
                // &(*x) = x
                dispatch(deref->operand.get());
            } else if (auto* member = dynamic_cast<MemberExprNode*>(node.operand.get())) {
                // &(p.x) = compute address of p.x
                compute_member_address(*member);
            } else {
                // For other expressions, evaluate and hope it's an lvalue
                dispatch(node.operand.get());
            }
            break;
        case OpKind::PRE_INC:
            if (auto* id = dynamic_cast<IdentifierExprNode*>(node.operand.get())) {
                if (local_variables_.count(id->name)) {
                    int offset = local_variables_[id->name];
                    emit("mov " + std::to_string(offset) + "(%rbp), %rax");
                    emit("add $1, %rax");
                    emit("mov %rax, " + std::to_string(offset) + "(%rbp)");
                } else {
                    int cap = find_capture_offset(id->name);
                    if (cap >= 0) {
                        emit("mov " + std::to_string(current_ctx_stack_offset_) + "(%rbp), %rdx");
                        emit("mov " + std::to_string(cap) + "(%rdx), %rax");
                        emit("add $1, %rax");
                        emit("mov %rax, " + std::to_string(cap) + "(%rdx)");
                    }
                }
            } else if (auto* member = dynamic_cast<MemberExprNode*>(node.operand.get())) {
                compute_member_address(*member);
                emit("push %rax");
                emit("mov (%rax), %rax");
                emit("add $1, %rax");
                emit("pop %rcx");
                emit("mov %rax, (%rcx)");
            }
            break;
        case OpKind::PRE_DEC:
            if (auto* id = dynamic_cast<IdentifierExprNode*>(node.operand.get())) {
                if (local_variables_.count(id->name)) {
                    int offset = local_variables_[id->name];
                    emit("mov " + std::to_string(offset) + "(%rbp), %rax");
                    emit("sub $1, %rax");
                    emit("mov %rax, " + std::to_string(offset) + "(%rbp)");
                } else {
                    int cap = find_capture_offset(id->name);
                    if (cap >= 0) {
                        emit("mov " + std::to_string(current_ctx_stack_offset_) + "(%rbp), %rdx");
                        emit("mov " + std::to_string(cap) + "(%rdx), %rax");
                        emit("sub $1, %rax");
                        emit("mov %rax, " + std::to_string(cap) + "(%rdx)");
                    }
                }
            } else if (auto* member = dynamic_cast<MemberExprNode*>(node.operand.get())) {
                compute_member_address(*member);
                emit("push %rax");
                emit("mov (%rax), %rax");
                emit("sub $1, %rax");
                emit("pop %rcx");
                emit("mov %rax, (%rcx)");
            }
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
                } else {
                    int cap = find_capture_offset(id->name);
                    if (cap >= 0) {
                        emit("mov " + std::to_string(current_ctx_stack_offset_) + "(%rbp), %rdx");
                        emit("mov " + std::to_string(cap) + "(%rdx), %rax");
                        emit("push %rax");
                        emit("add $1, %rax");
                        emit("mov %rax, " + std::to_string(cap) + "(%rdx)");
                        emit("pop %rax");
                    }
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
                } else {
                    int cap = find_capture_offset(id->name);
                    if (cap >= 0) {
                        emit("mov " + std::to_string(current_ctx_stack_offset_) + "(%rbp), %rdx");
                        emit("mov " + std::to_string(cap) + "(%rdx), %rax");
                        emit("push %rax");
                        emit("sub $1, %rax");
                        emit("mov %rax, " + std::to_string(cap) + "(%rdx)");
                        emit("pop %rax");
                    }
                }
            }
            break;
        default:
            error_message_ = "Unsupported unary operator";
            break;
    }
}

// Helper methods for struct/enum/typedef
int CodeGenerator::get_type_size(const std::string& type) {
    if (type.find('*') != std::string::npos) return 8;
    if (type == "int" || type == "const int") return 4;
    if (type == "char" || type == "const char") return 1;
    if (type == "bool" || type == "const bool") return 1;
    if (type == "void" || type == "const void") return 8;
    if (type == "long" || type == "const long") return 8;
    if (type == "short" || type == "const short") return 2;
    if (type == "float" || type == "const float") return 4;
    if (type == "double" || type == "const double") return 8;
    // Integer typedefs
    if (type == "size_t" || type == "ssize_t" || type == "ptrdiff_t" ||
        type == "intptr_t" || type == "uintptr_t" ||
        type == "uint64_t" || type == "int64_t") return 8;
    if (type == "uint32_t" || type == "int32_t" ||
        type == "unsigned int") return 4;
    if (type == "uint16_t" || type == "int16_t" ||
        type == "unsigned short") return 2;
    if (type == "uint8_t" || type == "int8_t") return 1;
    // Check if it's a struct type
    std::string clean = type;
    if (clean.substr(0, 7) == "struct ") clean = clean.substr(7);
    if (struct_layouts_.count(clean)) {
        return get_struct_size(clean);
    }
    return 8; // default
}

int CodeGenerator::get_struct_size(const std::string& name) {
    if (!struct_layouts_.count(name)) return 0;
    const auto& fields = struct_layouts_[name];
    if (fields.empty()) return 0;
    const auto& last = fields.back();
    return last.offset + last.size;
}

int CodeGenerator::get_field_offset(const std::string& struct_name, const std::string& field_name) {
    if (!struct_layouts_.count(struct_name)) return -1;
    for (const auto& f : struct_layouts_[struct_name]) {
        if (f.name == field_name) return f.offset;
    }
    return -1;
}

std::string CodeGenerator::get_struct_name(const std::string& type_name) {
    std::string s = type_name;
    // strip qualifiers
    auto strip = [&](const std::string& kw) {
        size_t p;
        while ((p = s.find(kw + " ")) != std::string::npos) s.erase(p, kw.size() + 1);
    };
    strip("const"); strip("volatile"); strip("static"); strip("extern");
    strip("register"); strip("auto"); strip("inline"); strip("restrict");
    if (s.substr(0, 7) == "struct ") s = s.substr(7);
    return s;
}

void CodeGenerator::visit(InitializerListNode& node) {
    // InitializerList is only meaningful as the initializer of a VarDeclNode,
    // which handles it specially. When evaluated as a generic expression,
    // we evaluate each element and leave the LAST one in %rax.
    for (auto& elem : node.elements) {
        if (auto* desig = dynamic_cast<DesignatedInitNode*>(elem.get())) {
            dispatch(desig->value.get());
        } else {
            dispatch(elem.get());
        }
    }
}

void CodeGenerator::visit(DesignatedInitNode& node) {
    // Standalone designated init: just compute the value
    dispatch(node.value.get());
}

void CodeGenerator::visit(CompoundLiteralNode& node) {
    // (type){init-list}: allocate hidden local storage and initialize it
    // Parse the type to figure out element size and array size
    // Supported forms: "int", "int[N]", "T*" (pointers), simple struct types
    int elem_size = 4;
    int array_size = 1;
    std::string elem_type = node.type_name;
    bool is_array = false;

    // Try to match "type[N]"
    size_t lb = node.type_name.find('[');
    if (lb != std::string::npos) {
        size_t rb = node.type_name.find(']', lb);
        if (rb != std::string::npos && rb > lb + 1) {
            elem_type = node.type_name.substr(0, lb);
            std::string size_str = node.type_name.substr(lb + 1, rb - lb - 1);
            array_size = std::atoi(size_str.c_str());
            if (array_size <= 0) array_size = 0;
            is_array = true;
        }
    }
    elem_size = get_type_size(elem_type);
    int alloc_size = elem_size * (array_size > 0 ? array_size : 1);

    // Allocate on stack
    int aligned_size = ((alloc_size + 7) / 8) * 8;
    if (aligned_size < 8) aligned_size = 8;
    stack_offset_ += aligned_size;
    int base_offset = -stack_offset_;

    // Initialize values from the InitializerList
    if (auto* init_list = dynamic_cast<InitializerListNode*>(node.initializer.get())) {
        int elem_index = 0;
        for (auto& elem : init_list->elements) {
            int slot = base_offset + elem_index * elem_size;
            if (auto* desig = dynamic_cast<DesignatedInitNode*>(elem.get())) {
                if (!desig->field_name.empty()) {
                    int field_off = get_field_offset(get_struct_name(node.type_name), desig->field_name);
                    if (field_off >= 0) slot = base_offset + field_off;
                } else if (desig->array_index >= 0) {
                    elem_index = desig->array_index;
                    slot = base_offset + elem_index * elem_size;
                }
                dispatch(desig->value.get());
            } else {
                dispatch(elem.get());
            }
            if (elem_size == 1) {
                emit("mov %al, " + std::to_string(slot) + "(%rbp)");
            } else if (elem_size == 4) {
                emit("movl %eax, " + std::to_string(slot) + "(%rbp)");
            } else {
                emit("mov %rax, " + std::to_string(slot) + "(%rbp)");
            }
            elem_index++;
        }
    }

    // Return address of the storage in %rax
    emit("lea " + std::to_string(base_offset) + "(%rbp), %rax");
}

// Nested function helpers
int CodeGenerator::find_capture_offset(const std::string& name) const {
    for (const auto& c : current_captures_) {
        if (c.name == name) {
            return c.ctx_offset;
        }
    }
    return -1;
}

void CodeGenerator::emit_pending_nested_function(PendingNestedFunc& pnf) {
    // Save enclosing function's state and set up the nested function's
    // own state.  The nested function has its own local_variables_
    // (initially empty), its own captures, and its own __ctx stack slot.
    auto saved_locals = local_variables_;
    auto saved_types = variable_types_;
    auto saved_arrays = array_info_;
    auto saved_captures = current_captures_;
    int saved_ctx_stack_offset = current_ctx_stack_offset_;
    auto saved_function = current_function_;
    auto saved_return_type = current_function_return_type_;

    local_variables_.clear();
    variable_types_.clear();
    array_info_.clear();
    current_captures_ = pnf.captures;
    current_ctx_stack_offset_ = 0;
    stack_offset_ = 0;
    returned_ = false;
    current_function_ = pnf.node->name;
    current_function_return_type_ = pnf.node->return_type;

    // Emit the function prologue manually because we need the hidden
    // __ctx parameter handling.
    emit(".globl " + pnf.node->name);
    emit_label(pnf.node->name);
    emit("push %rbp");
    emit("mov %rsp, %rbp");

    // Pre-allocate stack: 1 slot for __ctx + user params + locals + temps
    int local_count = 0;
    if (pnf.node->body) {
        auto& block = static_cast<BlockNode&>(*pnf.node->body);
        for (auto& stmt : block.statements) {
            if (stmt->type == NodeType::VAR_DECL) {
                local_count++;
            }
        }
    }
    int num_user_params = std::min((int)pnf.node->params.size(), 5);
    int total_slots = 1 + num_user_params + local_count + 16;  // +1 for __ctx
    int space = total_slots * 8;
    emit("sub $" + std::to_string(space) + ", %rsp");

    // Save __ctx (first arg, in %rdi) into a stack slot
    stack_offset_ += 8;
    current_ctx_stack_offset_ = -stack_offset_;
    emit("mov %rdi, " + std::to_string(current_ctx_stack_offset_) + "(%rbp)");

    // Map user parameters starting from %rsi (since %rdi is taken by __ctx)
    static const char* user_param_regs[] = {"%rsi", "%rdx", "%rcx", "%r8", "%r9"};
    for (int i = 0; i < num_user_params; i++) {
        stack_offset_ += 8;
        int offset = -stack_offset_;
        std::string pname = static_cast<ParamNode*>(pnf.node->params[i].get())->name;
        local_variables_[pname] = offset;
        variable_types_[pname] = static_cast<ParamNode*>(pnf.node->params[i].get())->type_name;
        emit("mov " + std::string(user_param_regs[i]) + ", " +
             std::to_string(offset) + "(%rbp)");
    }

    if (pnf.node->body) {
        dispatch(pnf.node->body.get());
    }

    if (!returned_) {
        emit("mov $0, %rax");
        emit_function_epilogue();
    }

    // Restore enclosing function's state
    local_variables_ = saved_locals;
    variable_types_ = saved_types;
    array_info_ = saved_arrays;
    current_captures_ = saved_captures;
    current_ctx_stack_offset_ = saved_ctx_stack_offset;
    current_function_ = saved_function;
    current_function_return_type_ = saved_return_type;
}

// Walk the AST statically to infer the type of an expression without
// generating code. The result is a string like "int", "float", "double",
// "char", or "<pointer-to-T>". Returns "int" as a safe default when the
// type cannot be determined.
std::string CodeGenerator::infer_expr_type(ASTNode* node) {
    if (!node) return "int";
    switch (node->type) {
        case NodeType::INTEGER_LITERAL: return "int";
        case NodeType::CHAR_LITERAL:    return "char";
        case NodeType::FLOAT_LITERAL: {
            auto* f = static_cast<FloatLiteralNode*>(node);
            return f->is_single_precision ? "float" : "double";
        }
        case NodeType::STRING_LITERAL:  return "char*";
        case NodeType::IDENTIFIER_EXPR: {
            auto* id = static_cast<IdentifierExprNode*>(node);
            auto it = variable_types_.find(id->name);
            if (it != variable_types_.end()) return it->second;
            return "int";
        }
        case NodeType::BINARY_EXPR: {
            auto* b = static_cast<BinaryExprNode*>(node);
            std::string lt = infer_expr_type(b->left.get());
            std::string rt = infer_expr_type(b->right.get());
            if (is_double_type(lt) || is_double_type(rt)) return "double";
            if (is_float_type(lt) || is_float_type(rt)) return "float";
            return lt.empty() ? "int" : lt;
        }
        case NodeType::UNARY_EXPR: {
            auto* u = static_cast<UnaryExprNode*>(node);
            if (u->op == OpKind::DEREF) {
                std::string inner = infer_expr_type(u->operand.get());
                if (inner.find('*') != std::string::npos) {
                    std::string pointee = inner;
                    size_t p = pointee.find('*');
                    while (p != std::string::npos) { pointee.erase(p, 1); p = pointee.find('*'); }
                    size_t s = pointee.find_first_not_of(" \t");
                    size_t e = pointee.find_last_not_of(" \t");
                    if (s != std::string::npos) pointee = pointee.substr(s, e - s + 1);
                    return pointee;
                }
            }
            if (u->op == OpKind::ADDRESS_OF) {
                std::string inner = infer_expr_type(u->operand.get());
                if (inner.empty()) inner = "int";
                return inner + "*";
            }
            return infer_expr_type(u->operand.get());
        }
        case NodeType::CAST_EXPR: {
            auto* c = static_cast<CastExprNode*>(node);
            return c->target_type;
        }
        case NodeType::CALL_EXPR: {
            auto* c = static_cast<CallExprNode*>(node);
            auto it = function_return_type_.find(c->function_name);
            if (it != function_return_type_.end()) return it->second;
            return "int";
        }
        case NodeType::ASSIGN_EXPR: {
            auto* a = static_cast<AssignExprNode*>(node);
            return infer_expr_type(a->target.get());
        }
        case NodeType::COMPOUND_ASSIGN_EXPR: {
            auto* a = static_cast<CompoundAssignExprNode*>(node);
            return infer_expr_type(a->target.get());
        }
        case NodeType::TERNARY_EXPR: {
            auto* t = static_cast<TernaryExprNode*>(node);
            std::string tt = infer_expr_type(t->then_expr.get());
            std::string et = infer_expr_type(t->else_expr.get());
            if (is_double_type(tt) || is_double_type(et)) return "double";
            if (is_float_type(tt) || is_float_type(et)) return "float";
            return tt.empty() ? "int" : tt;
        }
        case NodeType::COMMA_EXPR: {
            auto* c = static_cast<CommaExprNode*>(node);
            return infer_expr_type(c->right.get());
        }
        case NodeType::MEMBER_EXPR: {
            auto* m = static_cast<MemberExprNode*>(node);
            std::string obj_type = infer_expr_type(m->object.get());
            std::string sn = obj_type;
            if (sn.substr(0, 7) == "struct ") sn = sn.substr(7);
            if (struct_layouts_.count(sn)) {
                for (const auto& f : struct_layouts_[sn]) {
                    if (f.name == m->member) return f.type;
                }
            }
            return "int";
        }
        case NodeType::INDEX_EXPR: {
            auto* idx = static_cast<IndexExprNode*>(node);
            std::string inner = infer_expr_type(idx->array.get());
            if (inner.find('*') != std::string::npos) {
                std::string pointee = inner;
                size_t p = pointee.find('*');
                while (p != std::string::npos) { pointee.erase(p, 1); p = pointee.find('*'); }
                size_t s = pointee.find_first_not_of(" \t");
                size_t e = pointee.find_last_not_of(" \t");
                if (s != std::string::npos) pointee = pointee.substr(s, e - s + 1);
                return pointee;
            }
            return "int";
        }
        default:
            return "int";
    }
}

} // namespace simplecc
