#include <assert.h>
int main() {
    static_assert(sizeof(int) == 4, "int must be 4 bytes");
    return 0;
}
