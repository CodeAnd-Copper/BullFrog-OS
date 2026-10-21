
.section .rodata
message:
    .asciz "x86-64 Assembly module online!"

.section .text
.globl bullfrog_arch_name
.type bullfrog_arch_name, @function

bullfrog_arch_name:
    leaq message(%rip), %rax
    ret

.section .note.GNU-stack,"",@progbits
