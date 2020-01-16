.globl _start
.text
_start:
    xorl %eax, %eax
    incl %eax
    movb $1, %bl
    int $0x80

