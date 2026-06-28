# Lesson 0002: Abstract Syntax Tree (AST)

## Status: ✅ Complete | Phase: Core | Tests: 10

## Objective

Define AST node types for all parsed constructs.

## AST Node Types

```mermaid
classDiagram
    class ASTNode {
        +NodeType type
        +int line
        +int column
    }
    ASTNode <|-- ProgramNode
    ASTNode <|-- FunctionDeclNode
    ASTNode <|-- VarDeclNode
    ASTNode <|-- BlockNode
    ASTNode <|-- ReturnStmtNode
    ASTNode <|-- BinaryExprNode
    ASTNode <|-- UnaryExprNode
    ASTNode <|-- CallExprNode
    ASTNode <|-- IntegerLiteralNode
```

## Implemented Features

- All statement types (return, if, while, for, block)
- All expression types (binary, unary, call, assignment)
- All declaration types (function, variable, parameter)
- Visitor pattern for tree traversal
