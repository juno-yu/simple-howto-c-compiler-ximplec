struct Flags {
    unsigned int bold : 1;
    unsigned int italic : 1;
    unsigned int size : 6;
};

int main() {
    struct Flags f;
    f.bold = 1;
    return f.bold;
}
