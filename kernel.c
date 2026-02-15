void kmain(void) {
    volatile unsigned char* vga = (volatile unsigned char*)0xB8000;
    const char* msg = "Hello from C kernel!";

    for (int i = 0; msg[i] != '\0'; ++i) {
        vga[i * 2] = (unsigned char)msg[i];
        vga[i * 2 + 1] = 0x0A;
    }

    for (;;) {
        __asm__ __volatile__("hlt");
    }
}
