# Lesson 0023: Struct Member Access

## Status: 📋 Planned | Phase: Data Structures | Effort: Medium (6-8h)

## Objective

Implement `.` and `->` operators for struct access.

## Struct Access Operators

```mermaid
flowchart TD
    A[Struct Access] --> B[Dot Operator: s.member]
    A --> C[Arrow Operator: ptr->member]

    B --> D["Compute base address of struct"]
    D --> E["Add member offset"]
    E --> F["Load from base+offset"]

    C --> G["Load pointer value"]
    G --> H["Add member offset"]
    H --> I["Load from ptr+offset"]

    B --> J["s.x → lea offset(%rbp), %rax\nmov (%rax), %rax"]
    C --> K["ptr->x → mov ptr(%rbp), %rax\nmov offset(%rax), %rax"]
```

## Implementation Checklist

- [ ] Codegen for dot operator: `base + offset`
- [ ] Codegen for arrow operator: deref then add offset
- [ ] Handle nested member access: `s.point.x`
- [ ] Handle pointer members: `s->ptr`
- [ ] Struct assignment (memcpy)
- [ ] Test: `struct Point p; p.x = 10; return p.x;` → 10
