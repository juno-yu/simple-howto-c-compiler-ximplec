#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char *buf = malloc(64);
    int n = strlen("hello");
    memcpy(buf, "hello", n + 1);
    printf("len=%d buf=%s\n", n, buf);
    free(buf);
    exit(0);
}
