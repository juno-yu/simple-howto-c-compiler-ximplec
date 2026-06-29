#pragma once

#define assert(expr) ((expr) ? (void)0 : __assert_fail(#expr))
void __assert_fail(const char *expr);
