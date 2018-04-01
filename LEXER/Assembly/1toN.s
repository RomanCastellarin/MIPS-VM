.data
msg1:   .asciiz "Enter a number: "
msg2:   .asciiz "i="

.text
.globl main

main:
    # Show message
    li $v0, 4
    la $a0, msg1
    syscall
    
    # load number into $s0
    li $v0, 5
    syscall
    or $s0, $0, $v0
    
    # for( t0 = 0; t0 < s0; t0++)
    li $t0, 0
    for:
        beq $t0, $s0, endfor
        
        # Show message
        li $v0, 4
        la $a0, msg2
        syscall
        
        # Show number
        li $v0, 1
        or $a0, $0, $t0
        syscall
        
        # Show enter
        li $v0, 11
        li $a0, 10
        syscall
        
        addi $t0, $t0, 1
        j for
    endfor:

    li $v0, 10
    syscall
    