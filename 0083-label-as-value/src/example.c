int main() {
    int x = 0;
    goto *(&&target);
    x = 100;
    target:
    return x;
}
