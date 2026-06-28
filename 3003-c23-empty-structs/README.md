# Lesson 3003: Empty Structures (C23)

## Status: 📋 Planned | Standard: C23 | Effort: Easy

## Objective

Allow empty struct and union declarations.

## Syntax

```c
struct empty { };  // C23: valid, size = 1
union empty { };   // C23: valid, size = 1
```

## C11 vs C23

```c
// C11: empty struct is a constraint violation
struct empty { };  // error: invalid

// C23: empty struct is allowed
struct empty { };  // valid, sizeof(struct empty) == 1
```

## Implementation Checklist

- [ ] Allow empty struct/union declarations
- [ ] Set size to 1 byte (not 0)
- [ ] No members to access
- [ ] Allow array of empty structs
- [ ] Test: `struct empty { }; struct empty e; return sizeof(e);` → 1
