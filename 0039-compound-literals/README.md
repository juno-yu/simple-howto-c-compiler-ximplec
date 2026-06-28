# Lesson 0039: Compound Literals (C99)

## Status: 📋 Planned | Phase: Advanced Types | Effort: Medium (4-6h)

## Objective

Implement `(type){init-list}` for unnamed objects.

## Implementation Checklist

- [ ] Parse `(type){init-list}` expression syntax
- [ ] Create anonymous object in current scope
- [ ] Generate stack allocation for block-scope literals
- [ ] Support struct, array, and scalar compound literals
- [ ] Test: `return (struct Point){1, 2}.x;` → 1
