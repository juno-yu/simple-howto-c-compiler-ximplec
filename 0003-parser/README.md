# Lesson 0003: Recursive Descent Parser

## Status: ✅ Complete | Phase: Core | Tests: 20

## Objective

Parse token stream into AST using recursive descent.

## Grammar Rules

```mermaid
graph TD
    A[program] --> B[declaration*]
    B --> C[function_decl]
    B --> D[var_decl]
    C --> E[block]
    E --> F[statement*]
    F --> G[if_stmt | while_stmt | for_stmt | return_stmt | expr_stmt]
```

## Implemented Features

- Full expression precedence (15 levels)
- Function declarations with parameters
- Variable declarations with initializers
- Control flow: if/else, while, for
- Error reporting with line/column
