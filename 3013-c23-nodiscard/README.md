# Lesson 3013: [[nodiscard]] (C23)

## Status: 📋 Planned | Standard: C23 | Effort: Easy

## Objective

Warn when function return value is ignored.

## Syntax

```c
[[nodiscard]] int critical_operation(void);
[[nodiscard("error code")]] int error_prone(void);

// Warning: ignoring return value
critical_operation();  // compiler warns
```

## Use Cases

- Error codes: `[[nodiscard]] int fclose(FILE *);`
- Resource allocation: `[[nodiscard]] void *malloc(size_t);`
- Pure functions: `[[nodiscard]] int compute(int);`

## Implementation Checklist

- [ ] Parse `[[nodiscard]]` on functions
- [ ] Parse `[[nodiscard("msg")]]` with message
- [ ] Warn when return value is discarded
- [ ] Don't warn in void context: `(void)f();`
- [ ] Test: `[[nodiscard]] int f(void); f();` → warning
