int main() {
    // sizeof float/double should match C standard
    int sz1 = sizeof(float);
    int sz2 = sizeof(double);

    // Float literal bits should be preserved
    float f;
    int *p = (int*)&f;
    *p = 0;  // zero out
    f = 3.14f;
    int bits = *p;
    // 3.14f as float is 0x4048F5C3 = 1078523331

    // Return 1 if everything is correct, 0 otherwise
    if (sz1 != 4) return 0;
    if (sz2 != 8) return 0;
    if (bits != 1078523331) return 0;
    return 1;
}
