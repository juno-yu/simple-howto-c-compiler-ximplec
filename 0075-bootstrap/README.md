# Lesson 0075: Bootstrap and Verification

## Status: 📋 Planned | Phase: Self-Hosting | Effort: Hard

## Objective

Prove the compiler is correct via bootstrap verification.

## Bootstrap Process

```mermaid
graph TD
    A[gcc] --> B[compile simplecc source]
    B --> C[simplecc-v1]
    C --> D[compile simplecc source]
    D --> E[simplecc-v2]
    E --> F{C == E?}
    F -->|Yes| G[Verified!]
    F -->|No| H[Bug found]
```

## Implementation Checklist

- [ ] Build simplecc-v1 using gcc
- [ ] Build simplecc-v2 using simplecc-v1
- [ ] Compare outputs (should be identical or equivalent)
- [ ] Run full test suite on both
- [ ] Document verification results
- [ ] Celebration!
