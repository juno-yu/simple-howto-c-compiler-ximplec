#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdint.h>

int main() {
    char *buf = malloc(64);
    int n = strlen("hello");
    memcpy(buf, "hello", n + 1);
    printf("len=%d buf=%s\n", n, buf);
    
    double sq = sqrt(2.0);
    int c = toupper('a');
    int32_t big = INT32_MAX;
    
    free(buf);
    exit(0);
}
