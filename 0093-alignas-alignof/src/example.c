_Alignas(16) int aligned_var = 42;
int main() {
    int x = _Alignof(int);
    return x;
}
