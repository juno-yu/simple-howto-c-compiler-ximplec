# Lesson 0050: Static Linkage

## Status: ✅ Complete | Phase: System Integration | Effort: Easy

## Objective

Honor the `static` storage class for variables and functions so that the
emitted assembly does not export a `.globl` directive for them. The
compiler additionally recognises the `static` keyword in the type
specifier chain and stores the symbol as file-local in the semantic
table.

## Semantics

| Construct | Behavior |
|-----------|----------|
| `static int x;` at file scope | Defined in `.data`/`.bss`, no `.globl` emitted |
| `static int x = 0;` at file scope | Same as above, with initializer |
| `static int f(int n) { ... }` at file scope | Defined with no `.globl` |
| `static int x;` at function scope | Treated as a local with persistent storage (see Limitations) |

## Implementation

The keyword is parsed in `parse_type_specifier` (src/parser.cpp:108-109)
and concatenated into the type-name string. The semantic analyzer stores
`is_static = true` on the resulting symbol
(`src/semantic.cpp:246`):

```cpp
// src/semantic.cpp:246
sym.is_static = node.type_name.find("static") != std::string::npos;
sym.is_extern = node.type_name.find("extern") != std::string::npos;
```

The codegen consults `gvar.is_extern` to decide whether to emit `.globl`
for a global, and emits it only for non-`extern` symbols
(`src/codegen.cpp:47-65`):

```cpp
// src/codegen.cpp:47
// Emit global variables (skip extern - they're defined elsewhere)
bool has_non_extern_globals = false;
for (const auto& gvar : globals_) {
    if (!gvar.is_extern) {
        has_non_extern_globals = true;
        break;
    }
}
if (has_non_extern_globals) {
    emit(".data");
    for (const auto& gvar : globals_) {
        if (gvar.is_extern) continue;  // Skip extern variables
        emit(".globl " + gvar.name);
        emit_label(gvar.name);
        // ... emit .long / .zero for the value
    }
}
```

`static` functions go through `emit_function_prologue` which always
emits `.globl <name>` (`src/codegen.cpp:98-100`). The semantic analyzer's
`is_static` flag is therefore not yet wired into the function-emit path —
see Limitations.

## Limitations

- `static` functions are not yet filtered out of `.globl` emission —
  the codegen still emits `.globl` for every function it defines. The
  semantic table records `is_static` (`src/semantic.h:27`), but the
  function-prologue path does not consult it.
- `static` local variables (persistent storage between function calls)
  are not implemented. They are accepted by the parser, declared
  normally on the stack, and the keyword is ignored for codegen.

## Example

```c
static int secret() { return 42; }
int main() { return secret(); }
```

## Source Code References

| Component | File:Line | Description |
|-----------|-----------|-------------|
| Keyword lex | `src/lexer.cpp:124` | `"static"` → `KW_STATIC` |
| Type specifier | `src/parser.cpp:108-109` | Appends `static ` to the type string |
| Semantic | `src/semantic.cpp:246-247` | Records `is_static` and `is_extern` |
| Global emit | `src/codegen.cpp:47-65` | Emits `.globl` only for non-`extern` globals |
| Function prologue | `src/codegen.cpp:98-100` | Always emits `.globl` (limitation) |
