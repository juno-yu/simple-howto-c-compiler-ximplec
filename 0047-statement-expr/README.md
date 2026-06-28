# Lesson 0047: Statement Expressions (GCC Extension)

## Status: 📋 Planned | Phase: Float & Advanced | Effort: Medium (4-6h)

## Objective

Implement `{ stmt; expr; }` compound expressions.

## Statement Expression Processing

```mermaid
flowchart TD
    A["({ stmt1; stmt2; expr; })"] --> B[Parse block as expression]
    B --> C[Execute statements sequentially]
    C --> D[Evaluate final expression]
    D --> E[Return expression value]
    E --> F[Result typed as expression type]

    G["Nesting support"] --> H["Inner block evaluates independently"]
    G --> I["Value propagates outward"]
    G --> J["Side effects persist"]
```

## Implementation Checklist

- [ ] Parse `{ stmts...; expr; }` as expression
- [ ] Execute statements, return last expression value
- [ ] Support nesting
- [ ] Test: `int x = ({ int a = 1; a + 1; });` → 2
