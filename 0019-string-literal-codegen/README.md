# Lesson 0019: String Literal Code Generation

## Status: 📋 Planned | Phase: String & Memory | Effort: Easy (4-6h)

## Objective

Generate `.rodata` section for string constants.

## String Literal Codegen Flow

```mermaid
graph TD
    A["\"hello\" in source"] --> B["String literal table"]
    B --> C[".rodata section with label"]
    C --> D["lea .LC0(%rip), %rax"]
    D --> E["Load address into register"]
```

## Implementation Checklist

- [ ] Add string literal table to codegen
- [ ] Generate `.rodata` section with labeled strings
- [ ] Emit `lea label(%rip), %rax` to load string addresses
- [ ] Handle string escape sequences
- [ ] Test: `return "hello"[0];` → 104 ('h')
- [ ] Test: `char *s = "hi"; return s[1];` → 105 ('i')
