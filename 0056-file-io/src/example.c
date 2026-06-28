void *fopen(char *path, char *mode); int fclose(void *f); int main() { void *f = fopen("test.txt", "r"); if (f) fclose(f); return 0; }
