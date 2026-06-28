void *malloc(int size); void free(void *p); int main() { int *p = malloc(sizeof(int)); *p = 42; free(p); return 0; }
