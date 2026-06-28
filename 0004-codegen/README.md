# Lesson 0004: Code Generator

## Objective

Generate x86-64 assembly from the AST.

## Concepts

### Compilation Pipeline

```mermaid
graph LR
    A[AST] --> B[Code Generator]
    B --> C[Assembly .s]
    C --> D[Assembler]
    D --> E[Object .o]
    E --> F[Linker]
    F --> G[Executable]
```

### Stack-Based Code Generation

```mermaid
graph TD
    subgraph "Expression: 2 + 3 * 4"
        A[push 2] --> B[push 3]
        B --> C[push 4]
        C --> D[mul]
        D --> E[add]
        E --> F[pop result]
    end
```

### Register Allocation (Simplified)

```mermaid
graph LR
    subgraph "x86-64 Registers"
        RAX[RAX - return value]
        RDI[RDI - 1st arg]
        RSI[RSI - 2nd arg]
        RDX[RDX - 3rd arg]
        RCX[RCX - 4th arg]
        RBP[RBP - base pointer]
        RSP[RSP - stack pointer]
    end
```

### Function Call Convention

```mermaid
sequenceDiagram
    participant Caller
    participant Stack
    participant Callee
    
    Caller->>Stack: Push args (right to left)
    Caller->>Stack: Push return address
    Stack->>Callee: jmp function
    Callee->>Stack: Push rbp
    Callee->>Stack: Set rbp = rsp
    Callee->>Callee: Allocate locals
    Note over Callee: Execute body
    Callee->>Stack: Restore rsp, rbp
    Callee->>Stack: ret
```

## Implementation

### Files

| File | Purpose |
|------|---------|
| `src/codegen.h` | CodeGenerator class |
| `src/codegen.cpp` | Assembly generation logic |
| `src/assembly.h` | Assembly instruction helpers |
| `tests/test_codegen.cpp` | Unit tests |

### Generated Assembly Example

For `int main() { return 42; }`:

```asm
    .globl main
main:
    push %rbp
    mov %rsp, %rbp
    mov $42, %rax
    pop %rbp
    ret
```

### Code Generation Rules

```mermaid
flowchart TD
    A[Visit Function] --> B[Prologue]
    B --> C[Visit Body]
    C --> D[Epilogue]
    
    B --> E["push %rbp<br>mov %rsp, %rbp"]
    D --> F["pop %rbp<br>ret"]
    
    C --> G{Statement Type}
    G -->|Return| H[Generate expression<br>mov to %rax]
    G -->|If| I[Generate condition<br>cmp + jmp]
    G -->|While| J[Loop label<br>condition + body]
```

## Test Cases

1. **Simple return**: `return 42;` → generates `mov $42, %rax`
2. **Arithmetic**: `return 2 + 3;` → correct stack operations
3. **Variable access**: `int x = 5; return x;` → stack frame offsets
4. **Function call**: `foo();` → argument passing
5. **Control flow**: `if (x) { ... }` → conditional jumps

## Expected Output

```bash
$ echo 'int main() { return 42; }' > test.c
$ ./compiler test.c
$ cat test.s
    .globl main
main:
    push %rbp
    mov %rsp, %rbp
    mov $42, %rax
    pop %rbp
    ret
$ gcc -o test test.s && ./test; echo $?
42
```

## Implementation Details

### Source Code References
| Component | File | Lines | Description |
|-----------|------|-------|-------------|
| CodeGenerator class | src/codegen.h | 10-152 | Class declaration with visitor overrides |
| Constructor | src/codegen.cpp | 7-9 | Initializes code generator |
| generate() | src/codegen.cpp | 10-63 | Main entry: generates assembly for program |
| emit() | src/codegen.cpp | 65-67 | Writes assembly line to output |
| emit_label() | src/codegen.cpp | 69-71 | Writes label to output |
| emit_function_prologue() | src/codegen.cpp | 73-82 | Generates function prologue (push %rbp, mov %rsp, %rbp) |
| emit_function_epilogue() | src/codegen.cpp | 84-89 | Generates function epilogue (pop %rbp, ret) |
| push()/pop() | src/codegen.cpp | 91-97 | Stack manipulation helpers |
| new_label() | src/codegen.cpp | 99-101 | Generates unique labels |
| dispatch() | src/codegen.cpp | 103-233 | Routes AST nodes to appropriate visitor |
| visit(ProgramNode) | src/codegen.cpp | 235-255 | Generates code for all declarations |
| visit(FunctionDeclNode) | src/codegen.cpp | 257-302 | Function code generation with prologue/epilogue |
| visit(VarDeclNode) | src/codegen.cpp | 304-336 | Variable allocation and initialization |
| visit(ReturnStmtNode) | src/codegen.cpp | 495-505 | Return statement code generation |
| visit(IfStmtNode) | src/codegen.cpp | 513-536 | If statement with conditional jumps |
| visit(WhileStmtNode) | src/codegen.cpp | 538-562 | While loop code generation |
| visit(ForStmtNode) | src/codegen.cpp | 589-628 | For loop code generation |
| visit(BinaryExprNode) | src/codegen.cpp | 644-646 | Binary expression (delegates to generate_binary) |
| visit(AssignExprNode) | src/codegen.cpp | 652-706 | Assignment expression code generation |
| visit(CompoundAssignExprNode) | src/codegen.cpp | 708-783 | Compound assignment code generation |
| visit(TernaryExprNode) | src/codegen.cpp | 785-803 | Ternary operator code generation |
| visit(CallExprNode) | src/codegen.cpp | 838-854 | Function call code generation |
| visit(IndexExprNode) | src/codegen.cpp | 856-897 | Array indexing code generation |
| visit(MemberExprNode) | src/codegen.cpp | 899-905 | Struct member access |
| visit(IdentifierExprNode) | src/codegen.cpp | 942-968 | Identifier loading (local/global) |
| generate_expression() | src/codegen.cpp | 970-974 | Expression code generation dispatcher |
| generate_binary() | src/codegen.cpp | 976-1086 | Binary operation code generation |
| generate_unary() | src/codegen.cpp | 1088-1195 | Unary operation code generation |
| get_type_size() | src/codegen.cpp | 1197-1214 | Type size calculation |
