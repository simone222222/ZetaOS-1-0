.globl gdt_flush

// This function loads the GDT pointer and resets segment registers
gdt_flush:
    movl 4(%esp), %eax   // Get pointer (first argument)
    lgdt (%eax)
    mov $0x10, %ax       // Data segment selector (2nd entry * 8)
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss
    ljmp $0x08, $next    // Far jump to code segment selector (1st entry * 8)
next:
    ret
