#define CONCAT(a, b) a##b
#define STRINGIFY(x) #x
int main() {
    int xy = 42;
    return CONCAT(x, y);
}
