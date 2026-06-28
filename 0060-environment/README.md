# Lesson 0060: Environment Variables

## Status: 📋 Planned | Phase: Stdlib Tier B | Effort: Easy

## Objective

Access environment variables.

## Functions

| Function | Complexity |
|----------|------------|
| `getenv(name)` | Easy |
| `setenv(name, value, overwrite)` | Medium |
| `unsetenv(name)` | Medium |

## Implementation Checklist

- [ ] Access `environ` global variable
- [ ] Implement getenv: search environment array
- [ ] Implement setenv: add/modify environment entry
- [ ] Implement unsetenv: remove environment entry
- [ ] Test: `getenv("HOME")` returns home directory
