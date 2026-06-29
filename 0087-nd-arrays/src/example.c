int main() {
    // 3D array
    int a[2][3][4];
    a[1][2][3] = 99;
    int v1 = a[1][2][3];

    // 4D array
    int b[2][3][4][5];
    b[1][2][3][4] = 77;
    int v2 = b[1][2][3][4];

    // 2D still works
    int c[3][4];
    c[2][3] = 55;
    int v3 = c[2][3];

    return v1 + v2 + v3; // 99 + 77 + 55 = 231, lower 8 bits
}
