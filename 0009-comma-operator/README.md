# Lesson 0009: Comma Operator

## Status: 📋 Planned | Phase: Quick Wins | Effort: Easy (1-2h)

## Objective

Implement `expr1, expr2` for sequential evaluation.

## Implementation Checklist

- [ ] Add `CommaExprNode` to AST: `{ left, right }`
- [ ] Parse comma in `parse_expression()`
- [ ] Codegen: evaluate left, discard, evaluate right
- [ ] Test: `int a = (1, 2, 3);` → a = 3
- [ ] Test: `for (i = 0, j = 10; i < j; i++, j--) {}`
