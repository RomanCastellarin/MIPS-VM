.data
msg1:   .asciiz "Enter a number "
msg2:   .asciiz "Fibonacci[n] = "

.text
.globl main

# a0 -> argument
# ra -> return address
fib:
    ori $t0, $0, 1
    ble $a0, $t0, base
    
    # caso general
    addi $sp, $sp, -12
    sw $ra, 8($sp)
    sw $a0, 4($sp)
    
    addi $a0, $a0, -1
    jal fib
    
    sw $v0, 0($sp)
    lw $a0, 4($sp)
    addi $a0, $a0, -2
    jal fib
    
    lw $t0, 0($sp)
    add $v0, $v0, $t0
    
    lw $ra, 8($sp)
    
    addi $sp, $sp, 12
    j end_fib
    
    base:
    or $v0, $0, $a0
    
    end_fib:
    jr $ra

main:
    # Show message
    li $v0, 4
    la $a0, msg1
    syscall
    
    # load number into $a0
    li $v0, 5
    syscall
    or $a0, $0, $v0
    
    # call fibonacci($a0)
    jal fib
    or $s0, $0, $v0
    
    # Show message
    li $v0, 4
    la $a0, msg2
    syscall
     
    # Show number
    or $a0, $0, $s0
    li $v0, 1
    syscall

    li $v0, 10
    syscall
    