# Lesson 0012: const Qualifier

## Status: 📋 Planned | Phase: Quick Wins | Effort: Easy (3-4h)

## Objective

Implement `const` for read-only variables.

## Implementation Checklist

- [ ] Parse `const` keyword
- [ ] Add `is_const` flag to type/variable info
- [ ] Error on modification of const variables
- [ ] Support const pointers: `int *const p`
- [ ] Support pointer to const: `const int *p`
- [ ] Test: `const int x = 5; x = 10;` → compile error
