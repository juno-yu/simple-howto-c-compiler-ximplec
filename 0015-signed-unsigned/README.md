# Lesson 0015: Signed/Unsigned and Integer Sizes

## Status: 📋 Planned | Phase: Type System | Effort: Medium (4-6h)

## Objective

Implement `short`, `long`, `long long`, `unsigned` variants.

## Integer Type Sizes (x86-64)

| Type | Size | Register |
|------|------|----------|
| char | 1 | al/ax/eax/rax |
| short | 2 | ax/eax/rax |
| int | 4 | eax/rax |
| long | 8 | rax |
| long long | 8 | rax |
| unsigned * | same | zero-extended |

## Implementation Checklist

- [ ] Add types: short, unsigned short, unsigned int, long, unsigned long, long long
- [ ] Update type size calculations
- [ ] Generate proper width instructions (movb/movw/movl/movq)
- [ ] Handle sign extension: `movsbq`, `movzbq`, `movslq`
- [ ] Test: `unsigned int x = -1;` → 4294967295
