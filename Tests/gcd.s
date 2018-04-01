.data
    str1: .asciiz "Ingrese un numero positivo: " 
    str2: .asciiz "Ingrese otro: "
    str3: .asciiz "GCD: "
 
.text
    #bgt $s0,1,recur
    #bgt $s1,1,recur

main:   li $v0,4
        la $a0,str1
        syscall

        li $v0,5
        syscall

        move $s0,$v0
        li $v0,4
        la $a0,str2
        syscall

        li $v0,5
        syscall

        move $s1,$v0
        jal minimo
    


        move $a0,$v0
        li $v0,1
        syscall

        li $v0, 10
        syscall

minimo:
#base
        bne $s0,$s1,recur
        move $v0,$s0
        jr $ra

recur:
        addi $sp,$sp,-4
        sw $ra,0($sp)
        bgt $s0,$s1,amb
        bgt $s1,$s0,bma

amb:
        sub $s0,$s0,$s1
        jal  minimo
        j terminador

bma:
        sub $s1,$s1,$s0
        jal minimo  

terminador:
        lw $ra,0($sp)
        addi $sp,$sp,4
        jr $ra