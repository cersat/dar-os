# dar-os

Минимальный пример загрузочного сектора на x86 ASM, который можно запустить в QEMU и вывести текст на экран.

## Как собрать

```bash
nasm -f bin boot.asm -o boot.bin
```

## Как запустить

```bash
qemu-system-i386 -drive format=raw,file=boot.bin
```

После запуска на экране появится строка:

`Privet iz minimalnogo boot sektora!`
