int main() {
    int x = __builtin_expect(1, 1);
    int y = __builtin_popcount(255);
    return x + y;
}
