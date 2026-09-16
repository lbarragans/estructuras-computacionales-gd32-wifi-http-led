.section .text
    .align 2

    /*
     * Capa de aplicacion HTTP/LED en RISC-V puro.
     * Banco local: no implementa radio, SoftAP, DHCP, TCP ni lwIP.
     */
    .equ RCU_AHB1EN,           0x40023830
    .equ GPIOC_CTL,            0x40020800
    .equ GPIOC_OMODE,          0x40020804
    .equ GPIOC_OSPD,           0x40020808
    .equ GPIOC_PUD,            0x4002080C
    .equ GPIOC_BOP,            0x40020818
    .equ GPIOC_BC,             0x40020828
    .equ GPIOC_CLOCK_BIT,      0x00000004
    .equ PC13_BIT,             0x00002000
    .equ PC13_FIELD_MASK,      0x0C000000
    .equ PC13_OUTPUT_MODE,     0x04000000
    .equ PC13_SPEED_10MHZ,     0x04000000
    .equ SYSTIMER_CMP_LO,      0xD1000008
    .equ SYSTIMER_CMP_HI,      0xD100000C
    .equ SYSTIMER_MTIMECTL,    0xD1000FF8
    .equ ECLIC_INTIP7,         0xD200101C
    .equ ECLIC_INTIE7,         0xD200101D
    .equ ECLIC_INTATTR7,       0xD200101E
    .equ ECLIC_INTCTRL7,       0xD200101F
    .equ MODE_OFF,             0
    .equ MODE_ON,              1
    .equ MODE_SLOW,            2
    .equ MODE_FAST,            3

    .global main
main:
    /* GPIOC/PC13 activo en alto en la placa LCKFB validada. */
    li t0, RCU_AHB1EN
    lw t1, 0(t0)
    ori t1, t1, GPIOC_CLOCK_BIT
    sw t1, 0(t0)
    fence iorw, iorw
    li t0, GPIOC_CTL
    lw t1, 0(t0)
    li t2, PC13_FIELD_MASK
    not t2, t2
    and t1, t1, t2
    li t2, PC13_OUTPUT_MODE
    or t1, t1, t2
    sw t1, 0(t0)
    li t0, GPIOC_OMODE
    lw t1, 0(t0)
    li t2, PC13_BIT
    not t2, t2
    and t1, t1, t2
    sw t1, 0(t0)
    li t0, GPIOC_OSPD
    lw t1, 0(t0)
    li t2, PC13_FIELD_MASK
    not t2, t2
    and t1, t1, t2
    li t2, PC13_SPEED_10MHZ
    or t1, t1, t2
    sw t1, 0(t0)
    li t0, GPIOC_PUD
    lw t1, 0(t0)
    li t2, PC13_FIELD_MASK
    not t2, t2
    and t1, t1, t2
    sw t1, 0(t0)
    li t0, GPIOC_BOP
    li t1, PC13_BIT
    sw t1, 0(t0)
    la t0, g_led_is_on
    li t1, 1
    sw t1, 0(t0)
    la t0, g_led_mode
    li t1, MODE_SLOW
    sw t1, 0(t0)
    la t0, g_app_state
    li t1, 2                    /* AP_READY solo para banco de aplicacion. */
    sw t1, 0(t0)

    /* SysTimer 1 ms. */
    li t0, SYSTIMER_MTIMECTL
    li t1, 2
    sw t1, 0(t0)
    li t0, SYSTIMER_CMP_LO
    li t1, -1
    sw t1, 0(t0)
    li t0, SYSTIMER_CMP_HI
    sw zero, 0(t0)
    li t0, SYSTIMER_CMP_LO
    li t1, 40000
    sw t1, 0(t0)
    li t0, ECLIC_INTIP7
    sb zero, 0(t0)
    li t0, ECLIC_INTATTR7
    li t1, 2
    sb t1, 0(t0)
    li t0, ECLIC_INTCTRL7
    sb zero, 0(t0)
    li t0, ECLIC_INTIE7
    li t1, 1
    sb t1, 0(t0)
    li t0, 8
    csrs mstatus, t0
    li t1, 500
    la t0, g_next_request_ms
    sw t1, 0(t0)
    li t1, 100
    la t0, g_next_led_ms
    sw t1, 0(t0)

.Lmain_loop:
    la t0, g_systimer_ticks
    lw s0, 0(t0)

    /* Alimentar el parser con seis lineas GET cada 2000 ms. */
    la t0, g_next_request_ms
    lw t1, 0(t0)
    sub t2, s0, t1
    blt t2, zero, .Lled_check
    li t2, 2000
    add t1, s0, t2
    sw t1, 0(t0)
    la t0, g_request_index
    lw t1, 0(t0)
    slli t2, t1, 2
    la t3, request_table
    add t3, t3, t2
    lw a0, 0(t3)
    la t3, g_last_request
    sw a0, 0(t3)
    jal ra, apply_http_request
    la t0, g_http_requests
    lw t1, 0(t0)
    addi t1, t1, 1
    sw t1, 0(t0)
    la t0, g_clients_served
    lw t1, 0(t0)
    addi t1, t1, 1
    sw t1, 0(t0)
    la t0, g_request_index
    lw t1, 0(t0)
    addi t1, t1, 1
    li t2, 6
    bltu t1, t2, .Lstore_request_index
    li t1, 0
    la t3, g_request_cycles
    lw t2, 0(t3)
    addi t2, t2, 1
    sw t2, 0(t3)
.Lstore_request_index:
    sw t1, 0(t0)

.Lled_check:
    la t0, g_next_led_ms
    lw t1, 0(t0)
    sub t2, s0, t1
    blt t2, zero, .Lbackground
    addi t1, t1, 100
    sw t1, 0(t0)
    jal ra, led_mode_update
.Lbackground:
    la t0, g_background_iterations
    lw t1, 0(t0)
    addi t1, t1, 1
    sw t1, 0(t0)
    j .Lmain_loop

/* a0=request, a1=patron, a2=longitud. Devuelve a0=1 si coincide. */
match_prefix:
    li t0, 0
.Lmatch_loop:
    bgeu t0, a2, .Lmatch_yes
    add t1, a0, t0
    lbu t2, 0(t1)
    add t1, a1, t0
    lbu t3, 0(t1)
    bne t2, t3, .Lmatch_no
    addi t0, t0, 1
    j .Lmatch_loop
.Lmatch_yes:
    li a0, 1
    ret
.Lmatch_no:
    li a0, 0
    ret

/* a0 apunta a la primera linea HTTP. */
    .global apply_http_request
apply_http_request:
    addi sp, sp, -16
    sw ra, 12(sp)
    sw s1, 8(sp)
    mv s1, a0
    la a1, pattern_status
    li a2, 12
    jal ra, match_prefix
    bnez a0, .Lrequest_done       /* /status no cambia modo. */
    mv a0, s1
    la a1, pattern_on
    li a2, 8
    jal ra, match_prefix
    bnez a0, .Lcommand_on
    mv a0, s1
    la a1, pattern_off
    li a2, 9
    jal ra, match_prefix
    bnez a0, .Lcommand_off
    mv a0, s1
    la a1, pattern_toggle
    li a2, 12
    jal ra, match_prefix
    bnez a0, .Lcommand_toggle
    mv a0, s1
    la a1, pattern_slow
    li a2, 16
    jal ra, match_prefix
    bnez a0, .Lcommand_slow
    mv a0, s1
    la a1, pattern_fast
    li a2, 16
    jal ra, match_prefix
    beqz a0, .Lrequest_done
    li t1, MODE_FAST
    j .Lcommand_store
.Lcommand_slow:
    li t1, MODE_SLOW
    j .Lcommand_store
.Lcommand_toggle:
    la t0, g_led_is_on
    lw t1, 0(t0)
    xori t1, t1, 1              /* encendido -> OFF; apagado -> ON */
    j .Lcommand_store
.Lcommand_off:
    li t1, MODE_OFF
    j .Lcommand_store
.Lcommand_on:
    li t1, MODE_ON
.Lcommand_store:
    la t0, g_led_mode
    sw t1, 0(t0)
    la t0, g_led_elapsed_ms
    sw zero, 0(t0)
    la t0, g_led_commands
    lw t1, 0(t0)
    addi t1, t1, 1
    sw t1, 0(t0)
.Lrequest_done:
    lw s1, 8(sp)
    lw ra, 12(sp)
    addi sp, sp, 16
    ret

led_mode_update:
    la t0, g_led_mode
    lw t1, 0(t0)
    li t2, MODE_ON
    beq t1, t2, .Lled_on
    beqz t1, .Lled_off
    la t0, g_led_elapsed_ms
    lw t2, 0(t0)
    addi t2, t2, 100
    sw t2, 0(t0)
    li t3, 1000
    li t4, MODE_FAST
    bne t1, t4, .Lcheck_interval
    li t3, 250
.Lcheck_interval:
    bltu t2, t3, .Lled_return
    sw zero, 0(t0)
    la t0, g_led_is_on
    lw t1, 0(t0)
    xori t1, t1, 1
    sw t1, 0(t0)
    beqz t1, .Lled_hw_off
    j .Lled_hw_on
.Lled_on:
    la t0, g_led_elapsed_ms
    sw zero, 0(t0)
    la t0, g_led_is_on
    li t1, 1
    sw t1, 0(t0)
.Lled_hw_on:
    li t0, GPIOC_BOP
    li t1, PC13_BIT
    sw t1, 0(t0)
    ret
.Lled_off:
    la t0, g_led_elapsed_ms
    sw zero, 0(t0)
    la t0, g_led_is_on
    sw zero, 0(t0)
.Lled_hw_off:
    li t0, GPIOC_BC
    li t1, PC13_BIT
    sw t1, 0(t0)
.Lled_return:
    ret

    .global eclic_mtip_handler
eclic_mtip_handler:
    li t0, ECLIC_INTIP7
    sb zero, 0(t0)
    la t0, g_systimer_ticks
    lw t1, 0(t0)
    addi t1, t1, 1
    sw t1, 0(t0)
    ret

.section .rodata
pattern_status: .asciz "GET /status "
pattern_on:     .asciz "GET /on "
pattern_off:    .asciz "GET /off "
pattern_toggle: .asciz "GET /toggle "
pattern_slow:   .asciz "GET /blink/slow "
pattern_fast:   .asciz "GET /blink/fast "
request_status: .asciz "GET /status HTTP/1.1\r\n"
request_on:     .asciz "GET /on HTTP/1.1\r\n"
request_off:    .asciz "GET /off HTTP/1.1\r\n"
request_toggle: .asciz "GET /toggle HTTP/1.1\r\n"
request_slow:   .asciz "GET /blink/slow HTTP/1.1\r\n"
request_fast:   .asciz "GET /blink/fast HTTP/1.1\r\n"
    .align 2
request_table:
    .word request_status,request_on,request_off
    .word request_toggle,request_slow,request_fast

.section .bss
    .align 2
    .global g_app_state
g_app_state:              .word 0
    .global g_led_mode
g_led_mode:               .word 0
    .global g_led_is_on
g_led_is_on:              .word 0
    .global g_http_requests
g_http_requests:          .word 0
    .global g_led_commands
g_led_commands:           .word 0
    .global g_clients_served
g_clients_served:         .word 0
g_request_index:          .word 0
g_request_cycles:         .word 0
g_last_request:           .word 0
g_led_elapsed_ms:         .word 0
g_systimer_ticks:         .word 0
g_next_request_ms:        .word 0
g_next_led_ms:            .word 0
g_background_iterations: .word 0
