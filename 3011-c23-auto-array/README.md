# Lesson 3011: auto Arrays (C23)

## Status: ⚠️ Partial | Standard: C23 | Effort: Easy

## Objective

Infer array size and element type from initializer.

## How It Works

The bundled example in `3011-c23-auto-array/src/example.c` does **not** actually use `auto arr[]` — it uses `int arr[] = {1, 2, 3};`, which is a normal C99 inferred-size array. The `auto` keyword exists (lesson 3001) but does not flow into the array declarator: `auto arr[]` is parsed but the size is not deduced from the initializer list and the program segfaults at runtime.

```text
$ simplecc -S /tmp/test.c   (auto arr[] = {1, 2, 3};)
$ gcc -o x test.c.s
$ ./x
Segmentation fault
```

The first-decl slot is consumed in `parse_var_decl` (`src/parser.cpp:640-670`) as a single `[N]`. An empty `[]` after `auto` ends up as a zero-length array layout because no size is recorded.

## Syntax

```c
auto arr[] = {1, 2, 3};       // ⚠️ compiles, segfaults at runtime
int  arr[] = {1, 2, 3};       // ✅ plain C99 inferred-size array
```

## Limitations

- `auto arr[]` does not infer the element count.
- `auto` does not combine with the array declarator to produce a sized type.

## Source Code References

| Component | File:Line | Description |
|-----------|-----------|---|
| Array decl | `src/parser.cpp:640-670` | Reads one or more `[N]` brackets |
| `auto` keyword | `src/lexer.cpp:136` | `auto` → `KW_AUTO` |
| `auto` qualifier | `src/parser.cpp:114-115` | Appended to type |
