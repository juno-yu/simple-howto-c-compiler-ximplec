# Lesson 0036: Function Pointers

## Status: 📋 Planned | Phase: Advanced Types | Effort: Hard (8-12h)

## Objective

Implement function pointer types and callbacks.

## Implementation Checklist

- [ ] Parse function pointer declarations: `void (*handler)(int)`
- [ ] Parse function pointer typedefs
- [ ] Call through function pointer: `handler(42)`
- [ ] Function pointer as parameter
- [ ] Function pointer as return type
- [ ] Test: `void (*op)(int) = &print_int; op(42);`

## Architecture

```mermaid
flowchart TD
    A["Function Pointer Declaration"] --> B["Parse return type"]
    B --> C["Parse pointer name"]
    C --> D["Parse parameter types"]
    D --> E["Create function pointer type"]

    F["Function Pointer Assignment"] --> G["Look up target function"]
    G --> H["Store function address"]

    I["Function Pointer Call"] --> J["Load function address"]
    J --> K["Set up call arguments"]
    K --> L["Indirect call via pointer"]

    M["Function Pointer as Parameter"] --> N["Declare callback type"]
    N --> O["Pass function address"]
    O --> P["Invoke through parameter"]

    Q["Function Pointer as Return Type"] --> R["Factory function pattern"]
    R --> S["Select function at runtime"]
    S --> T["Return function pointer"]

    style E fill:#ffd,stroke:#333
    style L fill:#ffd,stroke:#333
```
