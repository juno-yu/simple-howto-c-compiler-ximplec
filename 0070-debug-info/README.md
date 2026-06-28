# Lesson 0070: Debug Information (DWARF)

## Status: ⚠️ Partial | Phase: Optimization | Effort: Hard

## Objective

Generate DWARF debug info for `gdb`/`lldb` so the compiled binary can be debugged at the source level.

## Implementation Status

| Feature | Status |
|---------|--------|
| Track source locations (line/column) in tokens | ✅ |
| Track source locations in AST nodes | ✅ |
| Propagate locations in error messages | ✅ |
| Emit `.debug_info` section | ❌ |
| Emit `.debug_line` section | ❌ |
| Emit `.debug_abbrev` section | ❌ |
| Map source lines to assembly addresses | ❌ |
| Describe types and variables | ❌ |

## Limitation

The compiler has the **infrastructure** for source locations: tokens carry `line` and `column`, AST nodes inherit this, and errors are reported with positions. However, **no DWARF directives** (`.loc`, `.file`, `.cfi_*`, etc.) are emitted into the assembly output. The resulting binaries are stripped of debug info and cannot be source-level debugged.

To enable debugging, the user can add `gcc -g` at assembly time, but this only adds empty debug sections; there is no source-to-address mapping in the assembly itself.

## Source Code References

| Component | File | Description |
|-----------|------|-------------|
| Token source locations | `src/token.h` | `Token` struct carries `line` and `column` |
| AST node locations | `src/ast.h` | `ASTNode` base stores `line` and `column` |
| Error propagation | `src/compiler.cpp` | `CompileResult` carries error positions |
| Codegen | `src/codegen.cpp` | `emit()` writes assembly — would be the place to add `.file`/`.loc` |

## Future Work

1. Emit `.file "name.c"` at the top of the assembly.
2. Emit `.loc 1 line column` before each statement.
3. Map source lines to instruction addresses via labels.
4. Emit `.cfi_start_proc` / `.cfi_end_proc` for call frame info.
5. Generate `.debug_info` entries for types and variables.
6. Use `.set` directives to control section layout.
