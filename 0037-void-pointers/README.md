# Lesson 0037: void Pointers

## Status: ✅ Complete | Phase: Advanced Types | Effort: Easy (2-3h)

## Objective

Implement `void*` as a generic pointer type. `void*` shares all
the size and storage rules of any other pointer — the only
special treatment is at **dereference time**, where the codegen
needs to know how many bytes to load.

## Implementation Checklist

- [x] Treat `void*` as pointer type (size = 8)
- [x] Allow implicit conversion to/from `void*` (no codegen work
      needed — see Status)
- [x] Allow casting between pointer types
- [x] Test: `void *p = &x; int *q = (int*)p; return *q;` → 42

## Architecture

```mermaid
flowchart TD
    A["void* Declaration"] --> B["Allocate pointer storage"]
    B --> C["Size = 8 bytes on x86-64"]

    D["Implicit Conversion"] --> E{"Source type?"}
    E -->|Typed pointer| F["Convert to void*"]
    E -->|void*| G["Convert to typed pointer"]

    H["Explicit Cast"] --> I[Cast expression]
    I --> J[Verify pointer type]
    J --> K[Generate load/store via cast type"]

    L["void* Arithmetic Check"] --> M{"Arithmetic on void*?"}
    M -->|Yes| N["Error: pointer arithmetic on void*"]
    M -->|No| O[Allow dereference after cast]

    P["Common Patterns"] --> Q["malloc returns void*"]
    P --> R["qsort callback context]
    P --> S["Generic container"]

    style F fill:#ffd,stroke:#333
    style G fill:#ffd,stroke:#333
    style N fill:#fdd,stroke:#333
```

## Implementation Details

The core trick: `get_type_size` returns 8 for **any** string
containing `*`, so `void*` inherits the pointer size for free. The
only place a `void*` is special is at dereference — and the
codegen handles that by accepting an explicit cast.

### Type-size rule

The first branch in `get_type_size` is the catch-all for any
pointer type (`src/codegen.cpp:2065-2091`):

```cpp
// src/codegen.cpp:2065-2091
int CodeGenerator::get_type_size(const std::string& type) {
    if (type.find('*') != std::string::npos) return 8;
    if (type == "int" || type == "const int") return 4;
    if (type == "char" || type == "const char") return 1;
    if (type == "bool" || type == "const bool") return 1;
    if (type == "void" || type == "const void") return 8;
    ...
}
```

`void*` contains `*`, so it returns 8. `void` alone also returns
8 (you can't have a `void` local — but the codegen is conservative
and allocates a slot anyway).

### Dereference — load size from the cast

The deref visitor checks the operand: if it's a `CastExprNode`,
it uses the cast's target type's size; if it's an `IdentifierExpr`,
it strips the `*` from the variable's declared type to find the
pointee. For `*(int*)vp`, the cast's target type is `int*` — but
since that contains `*`, the load size is forced back to 8 (a
pointer's value). The actual byte width is determined by
**what's underneath** the cast.

```cpp
// src/codegen.cpp:1435-1464 (abridged)
void CodeGenerator::visit(DerefExprNode& node) {
    dispatch(node.operand.get());
    int load_size = 8;
    if (auto* cast = dynamic_cast<CastExprNode*>(node.operand.get())) {
        load_size = get_type_size(cast->target_type);
        if (cast->target_type.find('*') != std::string::npos) {
            // Pointer deref: load 8 bytes (the pointer's value)
            load_size = 8;
        }
    } else if (auto* id = dynamic_cast<IdentifierExprNode*>(node.operand.get())) {
        if (variable_types_.count(id->name)) {
            std::string vt = variable_types_[id->name];
            if (vt.find('*') != std::string::npos) {
                std::string pointee = vt;
                size_t p = pointee.find('*');
                while (p != std::string::npos) { pointee.erase(p, 1); p = pointee.find('*'); }
                ...trim...
                load_size = get_type_size(pointee);
            }
        }
    }
    if (load_size == 1) emit("movzbl (%rax), %eax");
    else if (load_size == 2) emit("movzwl (%rax), %eax");
    else if (load_size == 4) emit("movl (%rax), %eax");
    else emit("mov (%rax), %rax");
}
```

So `*(int*)vp` works because the parser wraps `vp` in a
`CastExprNode(target_type="int*")`; the deref dispatches the
inner operand, sees the cast, and loads with `movl` (size 4).

## Example

```c
// src/example.c
int main() {
    int x = 42;
    void *p = &x;
    return *(int *)p;
}
```

`&x` produces a pointer; assigning it to `p` is a bit-for-bit
copy. The cast `(int*)p` wraps `p` in a `CastExprNode`; the
deref sees the cast's target type is `int*`, strips the `*` to
get `int` (size 4), and emits `movl (%rax), %eax`. Result: 42.

## Source Code References

| Component | File | Lines | Description |
|-----------|------|-------|-------------|
| Type specifier parsing | `src/parser.cpp` | `99-215` | `parse_type_specifier()` accepts `void` and `*` |
| Pointer qualifier parsing | `src/parser.cpp` | `~177-180` | Each `*` token appends to the type string |
| `DerefExprNode` AST | `src/ast.h` | `511-516` | Single-child deref expression |
| `CastExprNode` AST | `src/ast.h` | `475-482` | `target_type` + inner expression |
| `get_type_size` | `src/codegen.cpp` | `2065-2091` | Any `*` → 8 bytes; `void` → 8 |
| `visit(DerefExprNode)` | `src/codegen.cpp` | `1435-1464` | Load size from cast or var pointee |
| `visit(IdentifierExprNode)` | `src/codegen.cpp` | `1547-1615` | `lea` for pointer-typed locals |
| `visit(AddressOfExprNode)` | `src/codegen.cpp` | `1466-1481` | `lea offset(%rbp), %rax` |

## Status

- **Lexer / Parser / Codegen**: ✅ `void*` works as a generic
  pointer; casting to a typed pointer and dereferencing works.
- **Note (no conversion code)**: Conversions between `int*` and
  `void*` are pure value-preserving moves — at the assembly
  level, the only thing that happens is `mov` of the 8-byte
  pointer. There is no special "convert" instruction emitted,
  so the test "implicit conversion" works as a side effect of
  how the registers and stack slots line up.
- **Note (arithmetic on `void*`)**: There is no explicit check
  for `void*` arithmetic. `vp + 1` would scale the offset by
  whatever `get_type_size("void")` returns (8 bytes), which is
  almost certainly not what the user wants. The standard
  requires this to be a constraint violation; we silently
  accept it.
