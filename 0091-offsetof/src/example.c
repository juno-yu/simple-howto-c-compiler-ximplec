struct S { int x; int y; }; int main() { return __builtin_offsetof(struct S, y); }
