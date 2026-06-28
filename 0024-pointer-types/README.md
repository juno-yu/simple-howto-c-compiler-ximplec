# Lesson 0024: Pointer Types

## Status: 📋 Planned | Phase: Data Structures | Effort: Hard (8-12h)

## Objective

Implement `int*`, `char*`, `void*` pointer types.

## Pointer Operations

```mermaid
flowchart TD
    A[Pointer Operations] --> B[Address-of: &x]
    A --> C[Dereference: *p]
    A --> D[Pointer arithmetic: p + n]
    A --> E[Comparison: p == q]

    B --> F["lea offset(%rbp), %rax"]
    C --> G["mov (%rax), %rax"]
    D --> H["add n*sizeof(T), %rax"]
    E --> I["cmpq %rax, %rbx\nsete %al"]

    J["int x = 42;\nint *p = &x;\nreturn *p;"] --> K["Assembly:\nlea -4(%rbp), %rax\nmov %rax, -8(%rbp)\nmov -8(%rbp), %rax\nmov (%rax), %rax"]
```

## Implementation Checklist

- [ ] Parse pointer declarations: `int *p`, `char **argv`
- [ ] Address-of operator: `&x` → `lea offset(%rbp), %rax`
- [ ] Dereference operator: `*p` → `mov (%rax), %rax`
- [ ] Pointer comparison: `==`, `!=`, `<`, `>`
- [ ] NULL pointer support (0)
- [ ] Test: `int x = 42; int *p = &x; return *p;` → 42
