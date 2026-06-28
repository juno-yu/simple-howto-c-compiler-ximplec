int main() {
    int x = 42;
    void *p = &x;
    return *(int *)p;
}
