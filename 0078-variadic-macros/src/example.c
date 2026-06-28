#define SUM(...) __VA_ARGS__
#define LOG(msg, ...) msg
int main() {
    int x = SUM(1 + 2 + 3);
    return x;
}
