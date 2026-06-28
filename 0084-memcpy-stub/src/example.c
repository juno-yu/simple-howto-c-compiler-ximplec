void *memcpy(void *dst, const void *src, int n);
int main() {
    int src[3] = {1, 2, 3};
    int dst[3];
    memcpy(dst, src, sizeof(int) * 3);
    return dst[2];
}
