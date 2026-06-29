# Lesson 3006: #embed (C23)

## Status: ✅ Complete (file → integer list) | Standard: C23 | Effort: Medium

## Objective

Include binary file contents as a comma-separated integer list.

## How It Works

The preprocessor dispatches `#embed` to `handle_embed`, which reads the named file and emits one integer per byte:

```cpp
// src/preprocessor.cpp:145-147
} else if (dir_name == "embed") {
    // #embed "filename" - embed binary file contents
    output += handle_embed(dir_args) + "\n";
}
```

```cpp
// src/preprocessor.cpp:587-631
std::string Preprocessor::handle_embed(const std::string& args) {
    // Parse filename from args: "filename" or <filename>
    std::string filename;
    size_t start = args.find_first_not_of(" \t");
    if (start == std::string::npos) {
        error_message_ = "#embed: missing filename";
        return "";
    }
    if (args[start] == '"') {
        size_t end = args.find('"', start + 1);
        if (end == std::string::npos) {
            error_message_ = "#embed: unterminated filename";
            return "";
        }
        filename = args.substr(start + 1, end - start - 1);
    } else if (args[start] == '<') {
        // ... same shape for angle-bracket form ...
    } else {
        filename = args.substr(start);
        // Trim trailing whitespace
    }
    // Read binary file
    std::string content = read_file(filename);
    if (content.empty() && !error_message_.empty()) {
        return "";
    }
    // Convert to comma-separated integer values
    std::string result;
    for (size_t i = 0; i < content.size(); i++) {
        if (i > 0) result += ", ";
        result += std::to_string(static_cast<unsigned char>(content[i]));
    }
    return result;
}
```

The result string is then spliced into the surrounding token stream, exactly where a C23 `const unsigned char data[] = { #embed "f.bin" };` would have its initialiser.

## Syntax

```c
const unsigned char icon[] = {
    #embed "icon.png"
};
```

## Limitations

- No fallback list — the C23 form `, 0x00, 0xFF, ...` after the directive is **not** honoured. Anything after `#embed` on its own line is processed as the next preprocessor line.
- No `__has_embed` or limit/prefix/suffix parameters.
- Missing file is a hard error (no default).
- The bundled example in `3006-c23-preprocessor-embed/src/example.c` is a stub that does not use `#embed`; it just returns `0`.

## Source Code References

| Component | File:Line | Description |
|-----------|-----------|---|
| Directive dispatch | `src/preprocessor.cpp:145-147` | `#embed` → `handle_embed` |
| `handle_embed` | `src/preprocessor.cpp:587-631` | Parse filename, read bytes, format as ints |
| File reader | `src/preprocessor.cpp:533` | `Preprocessor::read_file` |
