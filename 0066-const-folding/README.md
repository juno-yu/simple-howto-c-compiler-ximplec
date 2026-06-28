# Lesson 0066: Constant Folding

## Status: 📋 Planned | Phase: Optimization | Effort: Easy

## Objective

Evaluate constant expressions at compile time.

## Constant Folding Pipeline

```mermaid
graph LR
    A[Source: 2+3] --> B[Tokenizer]
    B --> C[Parser: BinaryExpr]
    C --> D[Constant Folding Pass]
    D --> E[Integer Literal: 5]
    E --> F[Codegen: mov $5, %eax]
```

## Examples

```c
// Before optimization
int x = 2 + 3;      // → int x = 5;
int y = 10 * 5 + 1; // → int y = 51;
int z = sizeof(int); // → int z = 4;

// After optimization
mov $5, %eax    # instead of add instruction
```

## Implementation Checklist

- [ ] Fold integer arithmetic: `2 + 3` → `5`
- [ ] Fold comparisons: `5 > 3` → `1`
- [ ] Fold logical operations: `1 && 1` → `1`
- [ ] Fold sizeof expressions
- [ ] Fold constant ternary: `1 ? a : b` → `a`
- [ ] Test: `return 2 + 3;` → `mov $5, %rax`
