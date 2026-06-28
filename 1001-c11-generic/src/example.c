int main() {
    int x = _Generic(1, int: 1, char: 2, default: 0);
    return x;
}
