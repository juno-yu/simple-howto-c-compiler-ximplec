# Lesson 0090: Comma Expressions in For Loops

## Status: ✅ Complete | Phase: Control Flow | Effort: Easy

## Objective

Support comma expressions in for-loop init and update: `for (i=0, s=0; ...; i++, s+=i)`.

## Implementation

Changed `parse_for_stmt` to use `parse_expression()` (which handles commas) instead of `parse_expr_stmt()` for init and update. Also fixed empty for-loop body (`for(...);`) by handling standalone `;` in `parse_expr_stmt`.

## Example

```c
int main() {
    int i, s = 0;
    for (i = 0, s = 0; i < 3; i++, s += i);
    return s;  // 0+1+2+3 = 6
}
```
