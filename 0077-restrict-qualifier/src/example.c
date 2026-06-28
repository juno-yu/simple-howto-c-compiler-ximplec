void copy(int *restrict dst, int *restrict src, int n) {
    for (int i = 0; i < n; i++) dst[i] = src[i];
}
int main() {
    int a[3] = {1, 2, 3};
    int b[3];
    copy(b, a, 3);
    return b[2];
}
