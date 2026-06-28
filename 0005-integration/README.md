# Lesson 0005: Integration

## Objective

Combine all components into a working compiler with end-to-end testing.

## Concepts

### Full Pipeline

```mermaid
graph LR
    A[.c file] --> B[Tokenizer]
    B --> C[Parser]
    C --> D[AST]
    D --> E[Code Gen]
    E --> F[.s file]
    F --> G[as]
    G --> H[.o file]
    H --> I[ld]
    I --> J[executable]
    
    style A fill:#f9f,stroke:#333
    style J fill:#9f9,stroke:#333
```

### Error Handling Flow

```mermaid
flowchart TD
    A[Source Code] --> B{Tokenizer}
    B -->|Error| C[Report: line, column, message]
    B -->|OK| D{Parser}
    D -->|Error| C
    D -->|OK| E{Code Gen}
    E -->|Error| C
    E -->|OK| F[Output assembly]
    
    C --> G[Exit with error code]
```

### Test Strategy

```mermaid
graph TD
    subgraph "Unit Tests"
        U1[Tokenizer tests]
        U2[Parser tests]
        U3[AST tests]
        U4[Codegen tests]
    end
    
    subgraph "Integration Tests"
        I1[End-to-end compile + run]
        I2[Error message tests]
        I3[Multiple file tests]
    end
    
    U1 --> I1
    U2 --> I1
    U3 --> I1
    U4 --> I1
```

## Implementation

### Files

| File | Purpose |
|------|---------|
| `src/main.cpp` | CLI entry point |
| `src/compiler.h` | Orchestrator class |
| `src/compiler.cpp` | Pipeline management |
| `tests/test_integration.cpp` | End-to-end tests |
| `tests/test_files/*.c` | Test C programs |

### Compiler CLI

```bash
Usage: simplecc [options] <file.c>

Options:
    -o <file>     Output file (default: a.out)
    -S            Output assembly only
    -t            Print tokens
    -a            Print AST
    -h            Show help
```

### Integration Test Cases

| Test | Input | Expected Output |
|------|-------|-----------------|
| Echo return | `int main() { return 42; }` | Exit code 42 |
| Arithmetic | `return 2 + 3 * 4;` | Exit code 14 |
| Variables | `int x = 10; return x;` | Exit code 10 |
| If/else | `if (1) return 1; else return 2;` | Exit code 1 |
| While | `int i = 0; while(i < 5) i++; return i;` | Exit code 5 |
| Function | `int add(int a, int b) { return a + b; } int main() { return add(2, 3); }` | Exit code 5 |

## Running Full Test Suite

```bash
cd build
cmake ..
make
ctest --output-on-failure --verbose
```

## Example End-to-End Test

```cpp
TEST_CASE("Complete program", "[integration]") {
    const char* source = R"(
        int main() {
            int x = 10;
            int y = 20;
            return x + y;
        }
    )";
    
    Compiler compiler;
    auto result = compiler.compile(source);
    REQUIRE(result.exit_code == 30);
}
```
