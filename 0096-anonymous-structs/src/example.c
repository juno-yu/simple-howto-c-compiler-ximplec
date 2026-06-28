struct Point { int x; int y; };
struct Rect {
    struct {
        int left;
        int top;
    };
    struct {
        int right;
        int bottom;
    };
};
int main() {
    struct Rect r;
    r.left = 0;
    r.top = 0;
    r.right = 100;
    r.bottom = 100;
    return r.right;
}
