# dar-os

Минимальный пример ОС: загрузчик на ASM читает и запускает ядро, написанное на C.

## Что внутри

- `boot.asm` — boot sector (16-bit real mode), который:
  - читает с диска `kernel.bin` (сектора после boot sector),
  - переключается в protected mode,
  - прыгает на `0x10000`, где лежит ядро.
- `kernel.c` — очень простое C-ядро, пишет строку в VGA-текстовый буфер.

## Сборка

```bash
./build.sh
```

После сборки появятся:
- `boot.bin`
- `kernel.bin`
- `os-image.bin` (готовый образ для QEMU)

## Запуск в QEMU

```bash
qemu-system-i386 -drive format=raw,file=os-image.bin
```

На экране должен появиться текст:

`Hello from C kernel!`
