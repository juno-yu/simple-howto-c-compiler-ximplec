void *malloc(int size);
void free(void *ptr);
int main() {
    int *p = malloc(sizeof(int) * 10);
    p[0] = 42;
    free(p);
    return 0;
}
