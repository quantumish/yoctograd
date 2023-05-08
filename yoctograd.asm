    .struct 0
Var: 
Var.value:
    .struct Var + 0
Var.grad:
    .struct Var + 4
Var.op:
    .struct Var.grad + 4
Var.parent1:
    .struct Var.op + 4
Var.parent2:
    .struct Var.parent1 + 4
Var_size = . - Var

    .text

v_new:
    mov %rdi, %rbx
    mov $Var_size, %rdi
    call malloc
    movd %xmm0, (%rax)
    mov %rsi, -8(%rax)
    mov %rbx, -12(%rax)
    mov %rdx, -20(%rax)
    ret 

v_back:
    ret 

.globl v_new, v_back
