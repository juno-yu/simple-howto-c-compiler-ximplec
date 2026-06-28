# Lesson 0048: Inline Assembly

## Status: 📋 Planned | Phase: Float & Advanced | Effort: Medium (6-8h)

## Objective

Implement `asm("instruction")` for inline assembly.

## Implementation Checklist

- [ ] Parse `asm("...")` expression
- [ ] Emit raw assembly in output
- [ ] Support basic operand constraints
- [ ] Test: `asm("mov $42, %rax");` → return 42
