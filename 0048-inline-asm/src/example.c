int main() { int result; asm("mov $42, %eax" : "=a"(result)); return result; }
