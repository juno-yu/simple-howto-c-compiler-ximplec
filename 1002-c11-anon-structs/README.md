# Lesson 1002: Anonymous Structs (C11)

## Status: ✅ Complete | Standard: C11 | Effort: Medium

## Objective

Embed unnamed structs within structs.

## Syntax

```c
struct Outer {
    struct {          // anonymous struct
        int x;
        int y;
    };                // note: no name
    int z;
};

struct Outer o;
o.x = 10;  // access directly, not o.inner.x
```

## How It Works

The parser treats the inner `struct { ... }` as a normal struct decl with a synthetic name. The struct codegen then lays out the fields at successive offsets inside the outer struct, so the inner members are accessible directly via the outer struct's offset table.

```cpp
// src/parser.cpp:179-189 — anonymous struct in parse_type_specifier
} else if (match(TokenType::KW_STRUCT)) {
    if (check(TokenType::IDENTIFIER)) {
        result += "struct " + peek().value;
        advance();
    } else if (check(TokenType::LBRACE)) {
        // Anonymous struct — generate a synthetic name
        result += "struct _anon_" + std::to_string(pos_);
    } else {
        error("Expected struct name");
        return result;
    }
}
```

```cpp
// src/codegen.cpp:600-616 — visit(StructDeclNode)
void CodeGenerator::visit(StructDeclNode& node) {
    std::vector<FieldInfo> fields;
    int offset = 0;
    for (auto& field_ast : node.fields) {
        auto* field = static_cast<StructFieldNode*>(field_ast.get());
        int field_size = get_type_size(field->type_name);
        FieldInfo fi;
        fi.name = field->name;
        fi.type = field->type_name;
        fi.offset = offset;
        fi.size = field_size;
        fields.push_back(fi);
        offset += field_size;
    }
    struct_layouts_[node.name] = fields;
}
```

The outer struct then gets a flat layout where the anonymous struct's members sit at known offsets and are reachable via `MemberExprNode`.

## Example

```c
struct Rect {
    struct { int left; int top; };
    struct { int right; int bottom; };
};
int main() {
    struct Rect r;
    r.left = 0; r.right = 100;
    return r.right;  // returns 100
}
```

## Source Code References

| Component | File:Line | Description |
|-----------|-----------|-------------|
| Anonymous struct token | `src/parser.cpp:179-189` | Synthetic `_anon_<pos>` name |
| Struct layout | `src/codegen.cpp:600-616` | Sequential offset assignment |
| Field lookup | `src/codegen.cpp:2101-2107` | `get_field_offset` walks the flat table |
