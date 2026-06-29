# Lesson 0046: Variadic Functions

## Status: ⚠️ Partial | Phase: System Integration | Effort: Hard

## Objective

Parse and compile C functions declared with a variable-argument list (`...`). The
compiler accepts the `...` syntax and treats the variadic arguments as opaque —
the program can call the function and read its fixed leading arguments, but
there is no `va_list` / `va_arg` / `va_start` runtime support.

## What Works

- A parameter list ending in `...` parses successfully.
- The call site accepts any number of extra arguments and they are passed per
  the System V x86-64 ABI (the first 6 integer args in `%rdi..%r9`, float
  args in `%xmm0..%xmm7`).
- A function that ignores the variadic args (e.g. `int sum(int n, ...) { return n; }`)
  compiles, links, and runs — the value of `n` is correct because it travels in
  `%rdi` exactly as for a fixed-arg function.

## What Does Not Work

- `va_list`, `va_start`, `va_arg`, `va_end` are not implemented. A program
  cannot iterate the variadic arguments at runtime.
- The compiler does not generate the `EH_INFO` / `SAVE_ARGS` prologue that GCC
  emits for variadic callees. Calling a `variadic` C function that genuinely
  reads its varargs from `simplecc`-compiled code will misbehave.
- Variadic arguments are still passed on the call site (the ABI allows the
  callee to ignore them), so the function is just a regular C function as far
  as the linker is concerned.

## Variadic Parameter Parsing

```cpp
// src/parser.cpp:937
ASTPtr Parser::parse_param() {
    if (match(TokenType::ELLIPSIS)) {
        auto param = std::make_unique<ParamNode>(
            "...", "...", tokens_[pos_ - 1].line, tokens_[pos_ - 1].column);
        return std::move(param);
    }
    // ... regular typed parameter parsing
}
```

The `ELLIPSIS` token (`...`) is produced by the lexer at `src/lexer.cpp:484`
and is recognized as a valid parameter by `parse_param()`. The codegen
(`src/codegen.cpp:1300-1365`) treats the variadic argument the same as any
other integer argument and does not emit any special variadic prologue in
the callee.

## Example

```c
int sum(int n, ...) { return n; }
int main() { return sum(1, 2, 3); }   // returns 1 (n == 1)
```

The test program in `tests/test_variadic.cpp` covers five scenarios: parsing
of `...`, calling a variadic function with extra args, calling with zero
varargs, calling with mixed-type args, and a function that returns a constant.

## Source Code References

| Component | File:Line | Description |
|-----------|-----------|-------------|
| `ELLIPSIS` token | `src/lexer.cpp:484` | Lexer recognises `...` as a single token |
| `parse_param` | `src/parser.cpp:937-942` | Returns a `ParamNode` for `...` |
| Call site | `src/codegen.cpp:1201-1365` | Generates a normal `call` for variadic callees |
| Param name `...` | `src/parser.cpp:940` | The variadic parameter is named `"..."` |
