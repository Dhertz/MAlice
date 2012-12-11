.align 2
.global main

main:
    ldr r4, =50000
    bl init_io

    mov r0, #14
    bl make_output
    mov r0, #15
    bl make_output
    mov r0, #18
    bl make_output
    mov r0, #23
    bl make_output
    mov r0, #25
    bl make_output
    mov r0, #8
    bl make_output
    mov r0, #7
    bl make_output

loop:
    ldr r0, =.L1
    mov r1, #14
    bl printf
    mov r0, #14
    bl flash_note

    ldr r0, =.L1
    mov r1, #15
    bl printf
    mov r0, #15
    bl flash_note

    ldr r0, =.L1
    mov r1, #18
    bl printf
    mov r0, #18
    bl flash_note

    ldr r0, =.L1
    mov r1, #23
    bl printf
    mov r0, #23
    bl flash_note

    ldr r0, =.L1
    mov r1, #25
    bl printf
    mov r0, #25
    bl flash_note

    ldr r0, =.L1
    mov r1, #8
    bl printf
    mov r0, #8
    bl flash_note

    ldr r0, =.L1
    mov r1, #7
    bl printf
    mov r0, #7
    bl flash_note

    b loop

flash_note:
    stmfd  sp!, {fp, lr}
    mov r5, r0
    bl set_high
    mov r0, r4
    bl usleep
    mov r0, r5
    bl set_low
    mov r0, r4
    bl usleep
    ldmfd  sp!, {fp, pc}

.L1:
    .asciz "%i\n"
