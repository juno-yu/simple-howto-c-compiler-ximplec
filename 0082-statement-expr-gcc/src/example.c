int main() {
    int x = ({
        int a = 5;
        int b = 10;
        a + b;
    });
    return x;
}
