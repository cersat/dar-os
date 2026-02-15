# dar-os

Минимальный пример ОС: загрузчик на ASM читает и запускает ядро, написанное на C.

## Что внутри

- `boot.asm` — boot sector (16-bit real mode), который:
  - читает с диска `kernel.bin` (сектора после boot sector),
  - переключается в protected mode,
  - прыгает на `0x10000`, где лежит ядро.
- `kernel.c` — C-ядро с базовыми функциями экрана и клавиатуры.

- `kernel.c` теперь содержит базовые "стандартные" функции для ядра:
  - вывод символа/строки в VGA (`put_char`, `write_string`),
  - очистка экрана (`clear_screen`),
  - чтение клавиши с клавиатуры через порты PS/2 (`read_key`).
- `linker.ld` — скрипт линковки ядра по адресу `0x10000`.

## Сборка

```bash
./build.sh
```

Скрипт автоматически подбирает режим `ld`:
- `elf_i386` (обычный Linux binutils),
- или `i386pe` (часто в MinGW-средах).

После сборки появятся:
- `boot.bin`
- `kernel.elf`
- `kernel.bin`
- `os-image.bin` (готовый образ для QEMU)

## Запуск в QEMU

```bash
qemu-system-i386 -drive format=raw,file=os-image.bin
```

На экране должен появиться текст:

`Hello from C kernel!`
