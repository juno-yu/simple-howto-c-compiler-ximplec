int main() {
    int result;
    asm("mov $42, %eax");
    return 42;
}
