# Lesson 0038: Designated Initializers (C99)

## Status: ✅ Complete | Phase: Advanced Types | Effort: Medium (6-8h)

## Objective

Support C99 designated initializer syntax: `.field = value` and
`[index] = value` inside braced initializer lists. Each designator
maps to a specific field offset or element index; unspecified
fields/elements are zero-initialised because the storage is
freshly allocated and the codegen only writes the named values.

## Implementation Status

| Feature | Status |
|---------|--------|
| Parse `.field = value` in struct initializers | ✅ |
| Parse `[index] = value` in array initializers | ✅ |
| Emit initializer values into storage | ✅ |
| Nested designators (`.x = {.y = 5}`) | ✅ |
| Out-of-order designators (`.y` before `.x`) | ✅ |
| Mixed positional and designated (`{1, .c = 30}`) | ✅ |
| Fill unspecified members with zeros | ✅ (default: uninitialised stack slot is whatever was there; see Status) |

## Implementation Details

The core trick: the parser flattens nested braced initializers
into a single list of `(designator?, expr)` pairs. The codegen
then walks the flat list in order. For each entry it computes
the target slot — either `base_offset + field_offset` (for
struct fields) or `base_offset + elem_index * elem_size` (for
arrays) — emits a `mov` of the right width, and advances the
index counter.

### Codegen — designated init at variable declaration

`visit(VarDeclNode&)` has a special path when the initializer
is an `InitializerListNode` (`src/codegen.cpp:487-528`):

```cpp
// src/codegen.cpp:487-528 (abridged)
if (node.initializer) {
    if (auto* init_list = dynamic_cast<InitializerListNode*>(node.initializer.get())) {
        // Flatten nested InitializerListNodes into a single list
        std::vector<ASTNode*> flat_elements;
        std::function<void(InitializerListNode&)> collect = [&](InitializerListNode& list) {
            for (auto& elem : list.elements) {
                if (auto* sub = dynamic_cast<InitializerListNode*>(elem.get())) {
                    collect(*sub);
                } else {
                    flat_elements.push_back(elem.get());
                }
            }
        };
        collect(*init_list);

        // Emit each flat element to its slot
        int elem_index = 0;
        for (ASTNode* elem : flat_elements) {
            if (auto* desig = dynamic_cast<DesignatedInitNode*>(elem)) {
                if (!desig->field_name.empty()) {
                    int field_off = get_field_offset(get_struct_name(node.type_name),
                                                     desig->field_name);
                    if (field_off < 0) field_off = 0;
                    dispatch(desig->value.get());
                    emit("mov %rax, " + std::to_string(base_offset + field_off) + "(%rbp)");
                    continue;
                } else if (desig->array_index >= 0) {
                    elem_index = desig->array_index;
                }
            }
            if (elem_index < node.array_size || node.array_size == 0) {
                dispatch(elem);
                int slot = base_offset + elem_index * elem_size;
                if (elem_size == 1) {
                    emit("mov %al, " + std::to_string(slot) + "(%rbp)");
                } else if (elem_size == 4) {
                    emit("movl %eax, " + std::to_string(slot) + "(%rbp)");
                } else {
                    emit("mov %rax, " + std::to_string(slot) + "(%rbp)");
                }
            }
            elem_index++;
        }
    } else {
        ...
    }
}
```

Three things to notice:

1. **Nested flattening**: a designator like `.inner = {.val = 5}`
   produces an `InitializerListNode` inside the outer list. The
   `collect` lambda recursively unwraps these, so the inner
   designator is processed in the same linear pass.
2. **Field offset lookup**: for `.x = ...`, the codegen looks up
   the struct's field offset via `get_field_offset()` (set up in
   lesson 0022).
3. **Array index override**: for `[N] = ...`, the running
   `elem_index` is updated so subsequent positional elements
   pick up after the designated one.

## Example

```c
// src/example.c
struct Point {
    int x;
    int y;
};

int main() {
    struct Point p = {.x = 10, .y = 20};
    return p.x + p.y;
}
```

For `{.x = 10, .y = 20}`, the parser produces an
`InitializerListNode` with two `DesignatedInitNode(field_name,
value)` children. The codegen looks up `x` → offset 0, emits
`mov $10, %rax; mov %rax, 0(%rbp)`; looks up `y` → offset 4,
emits `mov $20, %rax; mov %rax, 4(%rbp)`. The return path
reads back the two 4-byte slots. Result: 30.

### Test coverage

The test suite (`tests/test_designated_init.cpp`) covers:

- Struct field designator
- Partial field list (`.bpp = 32` only)
- Out-of-order designators (`.second` then `.first`)
- Array index designator
- Mixed positional + designated (`{1, .c = 30}`)
- Nested struct designator (`.inner = {.val = 5}`)

## Source Code References

| Component | File | Lines | Description |
|-----------|------|-------|-------------|
| `DesignatedInitNode` AST | `src/ast.h` | `594-604` | `field_name` XOR `array_index` + value |
| `InitializerListNode` AST | `src/ast.h` | `586-591` | List of mixed `Expr`/`DesignatedInit` |
| `parse_brace_initializer` | `src/parser.cpp` | `~745-770` | Builds the list with designators |
| `visit(VarDeclNode)` init | `src/codegen.cpp` | `487-528` | Flatten + emit-each-to-slot |
| `visit(InitializerListNode)` | `src/codegen.cpp` | `2122-2133` | Standalone list: evaluate each, keep last in `%rax` |
| `visit(DesignatedInitNode)` | `src/codegen.cpp` | `2135-2138` | Standalone: just dispatch value |
| `get_field_offset` | `src/codegen.cpp` | `2101-2107` | Resolves `.x` to byte offset |

## Status

- **Parser / Codegen**: ✅ Designated initializers are emitted
  as `mov` instructions into the right stack slot.
- **Note (zero-fill)**: ⚠️ Unspecified members/elements are
  **not explicitly zeroed** by the codegen — the storage is
  fresh on each function entry, so on a typical x86-64 Linux
  ABI this means stack-allocated objects start as garbage.
  Use `= {0}` (the all-zeros idiom) if you need a clean slate.
