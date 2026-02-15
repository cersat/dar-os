#!/usr/bin/env bash
set -euo pipefail

gcc -m32 -ffreestanding -fno-pie -fno-stack-protector -nostdlib -c kernel.c -o kernel.o
ld -m elf_i386 -Ttext 0x10000 --oformat binary -e kmain kernel.o -o kernel.bin

kernel_size=$(stat -c%s kernel.bin)
kernel_sectors=$(( (kernel_size + 511) / 512 ))

nasm -f bin -d KERNEL_SECTORS=${kernel_sectors} boot.asm -o boot.bin
cat boot.bin kernel.bin > os-image.bin

echo "kernel.bin size: ${kernel_size} bytes (${kernel_sectors} sectors)"
echo "Created os-image.bin"
