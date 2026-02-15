#!/usr/bin/env bash
set -euo pipefail

gcc -m32 -ffreestanding -fno-pie -fno-stack-protector -nostdlib -c kernel.c -o kernel.o

ld_emulations=$(ld -V 2>/dev/null || true)
if echo "$ld_emulations" | grep -q "elf_i386"; then
  ld_mode="elf_i386"
elif echo "$ld_emulations" | grep -q "i386pe"; then
  ld_mode="i386pe"
else
  echo "Unsupported ld emulation. Need one of: elf_i386 or i386pe" >&2
  echo "$ld_emulations" >&2
  exit 1
fi

ld -m "$ld_mode" -T linker.ld -nostdlib -e _start kernel.o -o kernel.elf
objcopy -O binary kernel.elf kernel.bin

kernel_size=$(stat -c%s kernel.bin)
kernel_sectors=$(( (kernel_size + 511) / 512 ))

nasm -f bin -d KERNEL_SECTORS=${kernel_sectors} boot.asm -o boot.bin
cat boot.bin kernel.bin > os-image.bin

echo "ld emulation: ${ld_mode}"
echo "kernel.bin size: ${kernel_size} bytes (${kernel_sectors} sectors)"
echo "Created os-image.bin"
