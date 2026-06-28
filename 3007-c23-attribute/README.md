# Lesson 3007: Attributes (C23)

## Status: 📋 Planned | Standard: C23 | Effort: Medium

## Objective

Standardized attribute syntax `[[attr]]`.

## Syntax

```c
[[noreturn]] void abort(void);
[[nodiscard]] int important_func(void);
[[maybe_unused]] int x = 42;
[[deprecated]] void old_func(void);
[[fallthrough]] case 1:
```

## Standard Attributes

| Attribute | Purpose |
|-----------|---------|
| `[[noreturn]]` | Function never returns |
| `[[nodiscard]]` | Warn if return ignored |
| `[[maybe_unused]]` | Suppress unused warning |
| `[[deprecated]]` | Mark as deprecated |
| `[[fallthrough]]` | Suppress fallthrough warning |

## Implementation Checklist

- [ ] Parse `[[attribute]]` syntax
- [ ] Parse `[[namespace::attr]]` syntax
- [ ] Implement standard attributes
- [ ] Ignore unknown attributes
- [ ] Test: `[[nodiscard]] int f(void);` warns on ignored return
