struct Value {
    int type;
    union {
        int i;
        double d;
        char *s;
    };
};
int main() {
    struct Value v;
    v.type = 1;
    v.i = 42;
    return v.i;
}
