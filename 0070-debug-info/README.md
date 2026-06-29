# Lesson 0070: Debug Information (DWARF)

## Status: ⚠️ Partial | Phase: Optimization | Effort: Hard

## Objective

Generate DWARF debug info for `gdb`/`lldb` so the compiled binary
can be debugged at the source level.

## Implementation Status

| Feature | Status |
|---------|--------|
| Track source locations (line/column) in tokens | ✅ |
| Track source locations in AST nodes | ✅ |
| Propagate locations in error messages | ✅ |
| Emit `.file` directive at top of assembly | ❌ |
| Emit `.loc` directives before each statement | ❌ |
| Emit `.cfi_start_proc` / `.cfi_end_proc` for call frames | ❌ |
| Emit `.debug_info` section | ❌ |
| Emit `.debug_line` section | ❌ |
| Emit `.debug_abbrev` section | ❌ |
| Map source lines to assembly addresses | ❌ |
| Describe types and variables | ❌ |

## What Is There

The compiler has the **infrastructure** for source locations: tokens
carry `line` and `column` (`src/token.h:113-122`), AST nodes
inherit these via the `ASTNode` base class
(`src/ast.h:50-100`), and errors are reported with positions
(`src/parser.cpp:267-273`).

The compiler tracks error positions in the `CompileResult` returned
to the user, so the `simplecc` driver can print error messages
with file/line/column. It does **not** emit any DWARF directives
(`.file`, `.loc`, `.cfi_*`, etc.) into the assembly output. The
resulting binaries are stripped of debug info and cannot be
source-level debugged by `gdb`/`lldb`.

To enable debugging, the user can pass `-g` to `gcc` at assembly
time, but this only adds empty debug sections — there is no
source-to-address mapping in the assembly itself.

## Limitation

The compiler does not currently emit any DWARF directives into the
assembly. Source-level debugging of `simplecc`-compiled code is
therefore not possible.

## Source Code References

| Component | File:Line | Description |
|-----------|-----------|-------------|
| Token source locations | `src/token.h:113-122` | `Token` struct carries `line` and `column` |
| AST node locations | `src/ast.h:50-100` | `ASTNode` base stores `line` and `column` |
| Error propagation | `src/parser.cpp:267-273` | Parser errors include position |
| CompileResult | `src/compiler.h:15-35` | `error_line` / `error_filename` fields |
| Codegen | `src/codegen.cpp:90-96` | `emit()` writes assembly — would be the place to add `.file` / `.loc` |

## Future Work

1. Emit `.file "name.c"` at the top of the assembly.
2. Emit `.loc 1 line column` before each statement.
3. Map source lines to instruction addresses via labels.
4. Emit `.cfi_start_proc` / `.cfi_end_proc` for call frame info.
5. Generate `.debug_info` entries for types and variables.
6. Use `.set` directives to control section layout.
