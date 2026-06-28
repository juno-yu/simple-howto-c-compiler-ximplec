# Lesson 1013: Header `<stdnoreturn.h>` (C11)

## Status: 📋 Planned | Standard: C11 | Effort: Trivial

## Objective

Provide `noreturn` macro for `_Noreturn`.

## Usage

```c
#include <stdnoreturn.h>

noreturn void abort(void);
noreturn void exit(int status);
```

## Noreturn Function Flow

```mermaid
graph TD
    A["noreturn void func(void)"] --> B["Function marked _Noreturn"]
    B --> C["Compiler knows: no return path"]
    C --> D["No 'return' statement required"]
    D --> E["Caller must not use return value"]
    E --> F["UB if function returns"]
```

## Implementation

- Define `noreturn` as `_Noreturn`
- Simple macro in header
