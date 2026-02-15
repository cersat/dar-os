ENTRY(_start)

SECTIONS
{
  . = 0x10000;

  .text : {
    *(.text.start)
    *(.text*)
    *(.rodata*)
  }

  .data : {
    *(.data*)
  }

  .bss : {
    *(.bss*)
    *(COMMON)
  }
}
