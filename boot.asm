; Простой загрузочный сектор: выводит текст через BIOS и зависает.
; Сборка: nasm -f bin boot.asm -o boot.bin
; Запуск: qemu-system-i386 -drive format=raw,file=boot.bin

bits 16
org 0x7C00

start:
    xor ax, ax
    mov ds, ax
    mov si, message

.print_char:
    lodsb
    test al, al
    jz .hang

    mov ah, 0x0E
    mov bh, 0x00
    mov bl, 0x07
    int 0x10
    jmp .print_char

.hang:
    cli
    hlt

message db 'Privet iz minimalnogo boot sektora!', 0

times 510 - ($ - $$) db 0
dw 0xAA55
