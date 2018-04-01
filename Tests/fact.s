.data
    str:    .asciiz "Es "
    check:  .asciiz "Check..."
    str1:   .asciiz "Ingrese "

.text

main:

    la $a0,str1
    li $v0,4
    syscall

    li $v0,5
    syscall

    move $s0, $v0               #add $s0,$v0,$0

    jal fact

    move $a0,$v0
    li $v0,1
    syscall

    j final 


fact:
    li $t0, 1

    bgt $s0, $t0, recursion

    li $v0, 1
    jr $ra

recursion:
    addi $sp,$sp,-8
    sw $ra,0($sp)
    sw $s0,4($sp)
    addi $s0,$s0,-1
    jal fact

    lw $v1, 4($sp)
    lw $ra, 0($sp)
    addi $sp,$sp,8
    mul $v0,$v0,$v1
    jr $ra



final:

    li $v0,10
    syscall