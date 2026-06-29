# Lesson 3014: Predefined Macros (C23)

## Status: ✅ Complete (built-ins) | Standard: C23 | Effort: Easy

## Objective

Provide the predefined macros required by C23.

## How It Works

A small set of standard macros is defined inside `Preprocessor::process` at the start of every compilation:

```cpp
// src/preprocessor.cpp:25-32
macros_["__STDC__"]              = Macro("__STDC__", "1");
macros_["__STDC_VERSION__"]      = Macro("__STDC_VERSION__", "202311L");
macros_["__x86_64__"]            = Macro("__x86_64__", "1");
macros_["__linux__"]             = Macro("__linux__", "1");
macros_["NULL"]                  = Macro("NULL", "0");
macros_["true"]                  = Macro("true", "1");
macros_["false"]                 = Macro("false", "0");
macros_["__bool_true_false_are_defined"] = Macro("__bool_true_false_are_defined", "1");
```

`compiler.cpp` also re-asserts `__bool_true_false_are_defined` after building:

```cpp
// src/compiler.cpp:18
preprocessor_.define_macro("__bool_true_false_are_defined", "1");
```

The bundled example in `3014-c23-predef-macros/src/example.c` references `__STDC_VERSION__` and returns `0`. Note that `__STDC_VERSION__` is **not** consumed as a built-in by simplecc — it is a real identifier that the source never uses, so the example compiles and links.

## New Macros in C23

| Macro | Provided? | Description |
|-------|-----------|-------------|
| `__STDC_VERSION__` | ✅ (`202311L`) | C standard version |
| `__VA_OPT__` | ❌ | Optional comma in variadic macros |
| `__cplusplus` | ❌ | C++ compatibility (n/a in C) |

## Standard Macros

| Macro | Provided? | Description |
|-------|-----------|-------------|
| `__FILE__` | ❌ | Current filename |
| `__LINE__` | ❌ | Current line number |
| `__DATE__` | ❌ | Compilation date |
| `__TIME__` | ❌ | Compilation time |
| `__STDC__` | ✅ (`1`) | Conforming implementation |
| `__STDC_HOSTED__` | ❌ | Hosted implementation |

## Source Code References

| Component | File:Line | Description |
|-----------|-----------|---|
| Built-in macros | `src/preprocessor.cpp:25-32` | `__STDC__`, `__STDC_VERSION__`, `NULL`, `true`, `false`, etc. |
| Reinjection | `src/compiler.cpp:18, 34` | `__bool_true_false_are_defined` re-asserted |
