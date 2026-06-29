struct A { struct { int x; } inner; }; int main() { struct A a; a.inner.x = 5; return a.inner.x; }
