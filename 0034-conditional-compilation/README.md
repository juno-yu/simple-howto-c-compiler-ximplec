# Lesson 0034: Conditional Compilation

## Status: 📋 Planned | Phase: Preprocessor | Effort: Medium (6-8h)

## Objective

Implement `#ifdef`, `#ifndef`, `#if`, `#elif`, `#else`, `#endif`.

## Implementation Checklist

- [ ] Parse `#ifdef NAME` / `#ifndef NAME`
- [ ] Parse `#if constexpr_expr`
- [ ] Parse `#elif`, `#else`, `#endif`
- [ ] Support `defined(NAME)` operator
- [ ] Nested conditionals
- [ ] Handle `#pragma once`
- [ ] Test: `#ifdef DEBUG ... #else ... #endif`
