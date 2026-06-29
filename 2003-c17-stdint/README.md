# Lesson 2003: stdint.h (C17)

## Status: ⚠️ Partial | Standard: C17 | Effort: Easy

## Objective

Exact-width integer types and limit macros.

## How It Works

The header shim provides the limit macros only. The `intN_t` / `uintN_t` typedefs are intentionally absent because simplecc has no separate `int8_t` etc. AST type — the compiler would have to lower the typedef into a real type alias it understands.

```c
// lib/stdint.h
#define INT8_MIN  (-128)
#define INT8_MAX  127
#define UINT8_MAX 255
#define INT16_MIN (-32768)
#define INT16_MAX 32767
#define UINT16_MAX 65535
#define INT32_MIN (-2147483647-1)
#define INT32_MAX 2147483647
#define UINT32_MAX 4294967295U
#define INT64_MIN (-9223372036854775807-1)
#define INT64_MAX 9223372036854775167
#define UINT64_MAX 18446744073709551615U
```

The bundled example in `2003-c17-stdint/src/example.c` is a stub: it only uses `int` and `long`. There is no `<stdint.h>` in the include path used by the example, so the macros above are not exercised. The `intptr_t`/`uintptr_t` names are recognised by the parser as type-specifier starters (`src/parser.cpp:89-90`) but are not actually aliased to a real type.

## Types (intended)

| Type | Size |
|------|------|
| `int8_t`, `uint8_t` | 1 byte |
| `int16_t`, `uint16_t` | 2 bytes |
| `int32_t`, `uint32_t` | 4 bytes |
| `int64_t`, `uint64_t` | 8 bytes |
| `intptr_t`, `uintptr_t` | pointer size |
| `intmax_t`, `uintmax_t` | max width |

## What Works

| Feature | Status |
|---------|--------|
| Limit macros via `<stdint.h>` | ✅ When the header is on the include path |
| `int8_t` / `int32_t` / … typedefs | ❌ Not defined in `lib/stdint.h` |
| `intptr_t` / `uintptr_t` recognised as type names | ✅ (parser only) |

## Source Code References

| Component | File:Line | Description |
|-----------|-----------|---|
| Limit macros | `lib/stdint.h` | `INT*_MIN` / `INT*_MAX` / `UINT*_MAX` |
| Type-name recognition | `src/parser.cpp:89-90` | `intN_t`/`uintN_t`/`intptr_t` listed in `is_type_specifier` |
