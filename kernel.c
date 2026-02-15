typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_COLOR 0x0A

static volatile u8* const VGA = (volatile u8*)0xB8000;
static u16 cursor = 0;

static inline void outb(u16 port, u8 value) {
    __asm__ __volatile__("outb %0, %1" : : "a"(value), "Nd"(port));
}

static inline u8 inb(u16 port) {
    u8 value;
    __asm__ __volatile__("inb %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

static void move_cursor(void) {
    outb(0x3D4, 0x0F);
    outb(0x3D5, (u8)(cursor & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (u8)((cursor >> 8) & 0xFF));
}

static void clear_screen(void) {
    for (u16 i = 0; i < VGA_WIDTH * VGA_HEIGHT; ++i) {
        VGA[i * 2] = ' ';
        VGA[i * 2 + 1] = VGA_COLOR;
    }
    cursor = 0;
    move_cursor();
}

static void put_char(char c) {
    if (c == '\n') {
        cursor += (VGA_WIDTH - (cursor % VGA_WIDTH));
    } else {
        VGA[cursor * 2] = (u8)c;
        VGA[cursor * 2 + 1] = VGA_COLOR;
        ++cursor;
    }

    if (cursor >= VGA_WIDTH * VGA_HEIGHT) {
        cursor = 0;
    }

    move_cursor();
}

static void write_string(const char* s) {
    while (*s) {
        put_char(*s++);
    }
}

static char scancode_to_ascii(u8 scancode) {
    static const char table[128] = {
        0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=',
        '\b', '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']',
        '\n', 0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
        0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*',
        0, ' '
    };

    if (scancode >= 128) return 0;
    return table[scancode];
}

static char read_key(void) {
    for (;;) {
        if ((inb(0x64) & 1) == 0) {
            continue;
        }

        u8 scancode = inb(0x60);
        if (scancode & 0x80) {
            continue;
        }

        char c = scancode_to_ascii(scancode);
        if (c) {
            return c;
        }
    }
}

void kmain(void) {
    clear_screen();
    write_string("Hello from C kernel!\n");
    write_string("Type on keyboard, I will echo. ESC to halt.\n> ");

    for (;;) {
        char c = read_key();
        if (c == 27) {
            write_string("\nHalting...\n");
            break;
        }
        put_char(c);
    }

    for (;;) {
        __asm__ __volatile__("hlt");
    }
}
