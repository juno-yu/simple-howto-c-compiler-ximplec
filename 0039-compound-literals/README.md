# Lesson 0039: Compound Literals (C99)

## Status: ✅ Complete | Phase: Advanced Types | Effort: Medium (4-6h)

## Objective

Support C99 compound literal syntax: `(type){initializer-list}`.
A compound literal creates an unnamed object with the given type
and initial value, similar to a string literal but for any type.
The result of the expression is the **address** of that storage.

## Syntax

```c
int *p = (int[]){1, 2, 3};
struct Point p = (struct Point){.x = 10, .y = 20};
```

## Implementation Status

| Feature | Status |
|---------|--------|
| Parse `(type){...}` syntax | ✅ |
| Type-checked against expected type | ✅ (in semantic pass) |
| Allocate storage for the object | ✅ (stack-allocated) |
| Emit initializer values | ✅ (per-element `mov`) |
| Designated initializers inside | ✅ (`.field = v`) |
| Array size deduction (`(int[]){1, 2, 3}`) | ✅ (parser deduces from element count) |
| Use in expression context | ✅ |

## Implementation Details

The core trick: a compound literal is **a hidden local
variable** whose slot is allocated on the stack and whose
initializer is written to that slot. The codegen returns the
address of the slot as the value of the expression.

### Codegen — hidden local

`visit(CompoundLiteralNode&)` parses the type string to find
the element size and array length, allocates a stack slot,
and writes the initializer values into it
(`src/codegen.cpp:2140-2200`):

```cpp
// src/codegen.cpp:2140-2200 (abridged)
void CodeGenerator::visit(CompoundLiteralNode& node) {
    // (type){init-list}: allocate hidden local storage and initialize it
    int elem_size = 4;
    int array_size = 1;
    std::string elem_type = node.type_name;
    bool is_array = false;

    // Try to match "type[N]"
    size_t lb = node.type_name.find('[');
    if (lb != std::string::npos) {
        size_t rb = node.type_name.find(']', lb);
        if (rb != std::string::npos && rb > lb + 1) {
            elem_type = node.type_name.substr(0, lb);
            std::string size_str = node.type_name.substr(lb + 1, rb - lb - 1);
            array_size = std::atoi(size_str.c_str());
            if (array_size <= 0) array_size = 0;
            is_array = true;
        }
    }
    elem_size = get_type_size(elem_type);
    int alloc_size = elem_size * (array_size > 0 ? array_size : 1);

    // Allocate on stack
    int aligned_size = ((alloc_size + 7) / 8) * 8;
    if (aligned_size < 8) aligned_size = 8;
    stack_offset_ += aligned_size;
    int base_offset = -stack_offset_;

    // Initialize values from the InitializerList
    if (auto* init_list = dynamic_cast<InitializerListNode*>(node.initializer.get())) {
        int elem_index = 0;
        for (auto& elem : init_list->elements) {
            int slot = base_offset + elem_index * elem_size;
            if (auto* desig = dynamic_cast<DesignatedInitNode*>(elem.get())) {
                if (!desig->field_name.empty()) {
                    int field_off = get_field_offset(get_struct_name(node.type_name),
                                                     desig->field_name);
                    if (field_off >= 0) slot = base_offset + field_off;
                } else if (desig->array_index >= 0) {
                    elem_index = desig->array_index;
                    slot = base_offset + elem_index * elem_size;
                }
                dispatch(desig->value.get());
            } else {
                dispatch(elem.get());
            }
            if (elem_size == 1) {
                emit("mov %al, " + std::to_string(slot) + "(%rbp)");
            } else if (elem_size == 4) {
                emit("movl %eax, " + std::to_string(slot) + "(%rbp)");
            } else {
                emit("mov %rax, " + std::to_string(slot) + "(%rbp)");
            }
            elem_index++;
        }
    }

    // Return address of the storage in %rax
    emit("lea " + std::to_string(base_offset) + "(%rbp), %rax");
}
```

The function ends by emitting `lea base(%rbp), %rax` so the
expression's value is the address of the hidden storage. The
caller can then index into it, take its address, or pass it to
a function.

### Parser — `(type){...}` recognition

`parse_unary()` recognises `(type)expr` and, if the `expr`
position is a `{...}`, dispatches to compound-literal
construction. For `(int[]){1, 2, 3}`, the empty brackets
trigger size deduction from the initializer list
(`src/parser.cpp:1704-1744`):

```cpp
// src/parser.cpp:1704-1744 (abridged)
if (check(TokenType::LPAREN)) {
    size_t saved_pos = pos_;
    advance();
    if (is_type_specifier()) {
        std::string type = parse_type_specifier();
        if (match(TokenType::LBRACKET)) {
            if (check(TokenType::INTEGER)) {
                type += "[" + peek().value + "]";
                advance();
            } else {
                type += "[-1]";  // empty brackets marker
            }
            expect(TokenType::RBRACKET);
        }
        if (match(TokenType::RPAREN)) {
            if (check(TokenType::LBRACE)) {
                auto init = parse_brace_initializer();
                if (!init) return nullptr;
                // Empty brackets? Deduce size from initializer
                if (type.size() >= 4 && type.substr(type.size() - 4) == "[-1]") {
                    int count = 0;
                    if (auto* list = dynamic_cast<InitializerListNode*>(init.get())) {
                        for (auto& e : list->elements) {
                            if (dynamic_cast<DesignatedInitNode*>(e.get())) continue;
                            count++;
                        }
                    }
                    type = type.substr(0, type.size() - 4) + "[" + std::to_string(count) + "]";
                }
                return std::make_unique<CompoundLiteralNode>(
                    type, std::move(init), line, col);
            }
            ...
        }
    }
    pos_ = saved_pos;
}
```

## Example

```c
int main() {
    int *p = (int[]){1, 2, 3};
    return p[1];
}
```

The parser sees `(int[]){1, 2, 3}`, deduces size 3, builds a
`CompoundLiteralNode(type="int[3]", init)`. The codegen
allocates 12 bytes (rounded to 16), writes `1`, `2`, `3` to
the slots, and returns the address. `p[1]` then loads offset
4 and returns `2`.

## Source Code References

| Component | File | Lines | Description |
|-----------|------|-------|-------------|
| `CompoundLiteralNode` AST | `src/ast.h` | `607-614` | `type_name` + `initializer` |
| Parser recognition | `src/parser.cpp` | `1704-1744` | `(type){...}` in `parse_unary` |
| `parse_brace_initializer` | `src/parser.cpp` | `~745-770` | Builds `InitializerListNode` |
| `visit(CompoundLiteralNode)` | `src/codegen.cpp` | `2140-2200` | Allocates, initializes, returns address |
| `get_type_size` | `src/codegen.cpp` | `2065-2091` | Element size lookup |
| `get_field_offset` | `src/codegen.cpp` | `2101-2107` | Designated field offset (struct) |

## Status

- **Parser / Codegen**: ✅ Compound literals are parsed,
  allocated on the stack, initialized element-by-element, and
  the resulting address is returned as the expression's value.
  Designated initializers inside the brace list work as in
  lesson 0038.
