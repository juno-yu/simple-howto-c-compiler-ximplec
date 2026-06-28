#include <catch2/catch_test_macros.hpp>
#include "ast.h"

using namespace simplecc;

TEST_CASE("AST node types", "[ast]") {
    SECTION("Program node") {
        ProgramNode prog;
        REQUIRE(prog.type == NodeType::PROGRAM);
        REQUIRE(prog.declarations.empty());
    }
    
    SECTION("Function declaration node") {
        FunctionDeclNode func("int", "main", 1, 1);
        REQUIRE(func.type == NodeType::FUNCTION_DECL);
        REQUIRE(func.return_type == "int");
        REQUIRE(func.name == "main");
        REQUIRE(func.params.empty());
        REQUIRE(func.body == nullptr);
    }
    
    SECTION("Variable declaration node") {
        VarDeclNode var("int", "x", 1, 1);
        REQUIRE(var.type == NodeType::VAR_DECL);
        REQUIRE(var.type_name == "int");
        REQUIRE(var.name == "x");
        REQUIRE(var.initializer == nullptr);
    }
    
    SECTION("Parameter node") {
        ParamNode param("int", "argc", 1, 1);
        REQUIRE(param.type == NodeType::PARAM);
        REQUIRE(param.type_name == "int");
        REQUIRE(param.name == "argc");
    }
}

TEST_CASE("AST statement nodes", "[ast]") {
    SECTION("Block node") {
        BlockNode block(1, 1);
        REQUIRE(block.type == NodeType::BLOCK);
        REQUIRE(block.statements.empty());
    }
    
    SECTION("Return statement node") {
        ReturnStmtNode ret(1, 1);
        REQUIRE(ret.type == NodeType::RETURN_STMT);
        REQUIRE(ret.value == nullptr);
    }
    
    SECTION("If statement node") {
        IfStmtNode if_stmt(1, 1);
        REQUIRE(if_stmt.type == NodeType::IF_STMT);
        REQUIRE(if_stmt.condition == nullptr);
        REQUIRE(if_stmt.then_branch == nullptr);
        REQUIRE(if_stmt.else_branch == nullptr);
    }
    
    SECTION("While statement node") {
        WhileStmtNode while_stmt(1, 1);
        REQUIRE(while_stmt.type == NodeType::WHILE_STMT);
        REQUIRE(while_stmt.condition == nullptr);
        REQUIRE(while_stmt.body == nullptr);
    }
    
    SECTION("For statement node") {
        ForStmtNode for_stmt(1, 1);
        REQUIRE(for_stmt.type == NodeType::FOR_STMT);
        REQUIRE(for_stmt.init == nullptr);
        REQUIRE(for_stmt.condition == nullptr);
        REQUIRE(for_stmt.update == nullptr);
        REQUIRE(for_stmt.body == nullptr);
    }
}

TEST_CASE("AST expression nodes", "[ast]") {
    SECTION("Binary expression node") {
        BinaryExprNode bin(OpKind::ADD, 1, 1);
        REQUIRE(bin.type == NodeType::BINARY_EXPR);
        REQUIRE(bin.op == OpKind::ADD);
        REQUIRE(bin.left == nullptr);
        REQUIRE(bin.right == nullptr);
    }
    
    SECTION("Unary expression node") {
        UnaryExprNode unary(OpKind::UMINUS, 1, 1);
        REQUIRE(unary.type == NodeType::UNARY_EXPR);
        REQUIRE(unary.op == OpKind::UMINUS);
        REQUIRE(unary.operand == nullptr);
    }
    
    SECTION("Assignment expression node") {
        AssignExprNode assign(1, 1);
        REQUIRE(assign.type == NodeType::ASSIGN_EXPR);
        REQUIRE(assign.target == nullptr);
        REQUIRE(assign.value == nullptr);
    }
    
    SECTION("Call expression node") {
        CallExprNode call("printf", 1, 1);
        REQUIRE(call.type == NodeType::CALL_EXPR);
        REQUIRE(call.function_name == "printf");
        REQUIRE(call.arguments.empty());
    }
}

TEST_CASE("AST literal nodes", "[ast]") {
    SECTION("Integer literal node") {
        IntegerLiteralNode int_lit(42, 1, 1);
        REQUIRE(int_lit.type == NodeType::INTEGER_LITERAL);
        REQUIRE(int_lit.value == 42);
    }
    
    SECTION("Float literal node") {
        FloatLiteralNode float_lit(3.14, 1, 1);
        REQUIRE(float_lit.type == NodeType::FLOAT_LITERAL);
        REQUIRE(float_lit.value == 3.14);
    }
    
    SECTION("String literal node") {
        StringLiteralNode str_lit("hello", 1, 1);
        REQUIRE(str_lit.type == NodeType::STRING_LITERAL);
        REQUIRE(str_lit.value == "hello");
    }
    
    SECTION("Char literal node") {
        CharLiteralNode char_lit('a', 1, 1);
        REQUIRE(char_lit.type == NodeType::CHAR_LITERAL);
        REQUIRE(char_lit.value == 'a');
    }
    
    SECTION("Identifier expression node") {
        IdentifierExprNode id("x", 1, 1);
        REQUIRE(id.type == NodeType::IDENTIFIER_EXPR);
        REQUIRE(id.name == "x");
    }
}

TEST_CASE("AST node type names", "[ast]") {
    REQUIRE(strcmp(node_type_name(NodeType::PROGRAM), "Program") == 0);
    REQUIRE(strcmp(node_type_name(NodeType::FUNCTION_DECL), "FunctionDecl") == 0);
    REQUIRE(strcmp(node_type_name(NodeType::VAR_DECL), "VarDecl") == 0);
    REQUIRE(strcmp(node_type_name(NodeType::BLOCK), "Block") == 0);
    REQUIRE(strcmp(node_type_name(NodeType::RETURN_STMT), "ReturnStmt") == 0);
    REQUIRE(strcmp(node_type_name(NodeType::BINARY_EXPR), "BinaryExpr") == 0);
    REQUIRE(strcmp(node_type_name(NodeType::INTEGER_LITERAL), "IntegerLiteral") == 0);
}

TEST_CASE("AST operator kind names", "[ast]") {
    REQUIRE(strcmp(op_kind_name(OpKind::ADD), "+") == 0);
    REQUIRE(strcmp(op_kind_name(OpKind::SUB), "-") == 0);
    REQUIRE(strcmp(op_kind_name(OpKind::MUL), "*") == 0);
    REQUIRE(strcmp(op_kind_name(OpKind::DIV), "/") == 0);
    REQUIRE(strcmp(op_kind_name(OpKind::EQ), "==") == 0);
    REQUIRE(strcmp(op_kind_name(OpKind::NE), "!=") == 0);
    REQUIRE(strcmp(op_kind_name(OpKind::LT), "<") == 0);
    REQUIRE(strcmp(op_kind_name(OpKind::GT), ">") == 0);
}

TEST_CASE("AST ownership and destruction", "[ast]") {
    SECTION("Program owns its children") {
        auto prog = std::make_unique<ProgramNode>();
        auto func = std::make_unique<FunctionDeclNode>("int", "main", 1, 1);
        auto body = std::make_unique<BlockNode>(1, 1);
        
        func->body = std::move(body);
        prog->declarations.push_back(std::move(func));
        
        REQUIRE(prog->declarations.size() == 1);
        REQUIRE(prog->declarations[0] != nullptr);
    }
    
    SECTION("Deep tree construction") {
        auto prog = std::make_unique<ProgramNode>();
        auto func = std::make_unique<FunctionDeclNode>("int", "main", 1, 1);
        auto body = std::make_unique<BlockNode>(1, 1);
        auto ret = std::make_unique<ReturnStmtNode>(1, 1);
        auto bin = std::make_unique<BinaryExprNode>(OpKind::ADD, 1, 1);
        auto left = std::make_unique<IntegerLiteralNode>(1, 1, 1);
        auto right = std::make_unique<IntegerLiteralNode>(2, 1, 1);
        
        bin->left = std::move(left);
        bin->right = std::move(right);
        ret->value = std::move(bin);
        body->statements.push_back(std::move(ret));
        func->body = std::move(body);
        prog->declarations.push_back(std::move(func));
        
        REQUIRE(prog->declarations.size() == 1);
        auto& main_func = static_cast<FunctionDeclNode&>(*prog->declarations[0]);
        REQUIRE(main_func.name == "main");
        REQUIRE(main_func.body != nullptr);
    }
}
