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
