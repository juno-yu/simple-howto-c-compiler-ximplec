int main() {
    // Test 1: float add produces correct bits
    float a = 1.0f, b = 2.0f;
    float c = a + b;
    int *p = (int*)&c;
    if (*p != 0x40400000) return 1;

    // Test 2: double mul
    double d = 2.0 * 3.0;
    long *lp = (long*)&d;
    if (*lp != 0x4018000000000000L) return 2;

    // Test 3: int + float promotion
    float e = 1.0f + 2;
    int *ep = (int*)&e;
    if (*ep != 0x40400000) return 3;

    // Test 4: float comparison
    if (!(1.0f < 2.0f)) return 4;

    // Test 5: float to int cast
    if ((int)3.7f != 3) return 5;

    // Test 6: int to float cast
    float f = (float)42;
    int *fp = (int*)&f;
    if (*fp != 0x42280000) return 6;

    // Test 7: float subtraction
    float g = 5.0f - 2.0f;
    int *gp = (int*)&g;
    if (*gp != 0x40400000) return 7;

    // Test 8: float division
    float h = 6.0f / 2.0f;
    int *hp = (int*)&h;
    if (*hp != 0x40400000) return 8;

    return 0;
}
