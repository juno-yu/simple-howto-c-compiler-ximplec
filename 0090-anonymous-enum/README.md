# Lesson 0093: Anonymous Enums

## Status: ⚠️ Partial | Phase: Advanced Types | Effort: Easy

## Objective

Support `enum {A=10, B=20, C=30};` without a name.

## Status

| Feature | Status |
|---------|--------|
| Named enum `enum Color {RED, GREEN}` | ✅ |
| Anonymous enum `enum {A=10, B=20}` | ⚠️ Parsed but not added to constants |

## Limitation

Anonymous enums are parsed but the values are not added to the enum_constants_ map, so `B` is not resolved.

## Example (works only with named enums)

```c
int main() {
    enum {A=10, B=20, C=30};  // parsed, B not resolved
    return B;  // fails
}
```
