.data

str:    .asciiz "abcde"      # cad
coma:   .asciiz ", "

.text


main:
    
    la $v1, str     # Puntero a primer lugar de cadena

    li $s2, 0       # int l = 0 inicialmente
    jal tofori
    j salir


init_strlen: 
            li $t1, 0       # Inicio contador de caracteres en 0
            la $a0, str     # Se guarda el primer lugar en variable a usar en strlen
            lb $t0, 0($a0)      # Se carga la primer letra de la cadena para el strlen

strlen: beq $t0, $0, exit
    addi $t1, $t1, 1
    addi $a0, $a0, 1
    lb $t0, 0($a0)
    j strlen
exit: jr $ra



tofori:

    addi $sp, $sp, -4
    sw $ra, 0($sp)
    jal init_strlen
    move $s0, $t1       # int n = strlen(cad). El n es fijo
    li $s1, 2       # $s1 = 2. Esto tambien es fijo
    move $t2, $s2   #
    li $t0, 0       # contador for


for_i:
    sub $t1, $s0, $t2   # $t1 = n-l. Debe estar dentro del for para tomar siempre el l actualizado
    bge $t0, $t1, end_i # if i >= n-l then endfor
    bgt $t1, $s1, rec   # if n-l > 2 then permutaciones(cad, l+1)
    
    li $v0, 4           # else mostrar
    move $a0, $v1
    syscall
    li $v0, 4
    la $a0, coma
    syscall
    j co

rec:
    addi $s2, $t2, 1    # $s2 seria el l del for donde estemos parados, $t2 el del for previo a la recursividad
    addi $sp, $sp, -8
    sw $t0, 0($sp)
    sw $t2, 4($sp)

    jal tofori      # permutaciones(cad, l+1)

    lw $t0, 0($sp)
    lw $t2, 4($sp)
    addi $sp, $sp, 8


co: add $t3, $v1, $t2   # $t3 apunta a cad + l

    lb $t4, 0($t3)                  # $t4 = *(cad + l)
    add $t5, $t3, $t0               # $t5 apunta a (cad + l) + i
    addi $t5, $t5, 1                # $t5 apunta a (cad + l) + i + 1
    lb $t6, 0($t5)                  # $t6 = *(cad + l + i + 1)
    sb $t4, 0($t5)                  # (cad + l + i + 1) = *(cad + l)
    sb $t6, 0($t3)                  # (cad + l) = *(cad + l + i + 1)

    addi $t4, $s0, -1   # $t4 = n-1

    add $t3, $t2, $t0   # $t3 = l + i
    bne $t3, $t4, end_j2    # if l+i != n-1 then endfor else for (j = l; j < n; j++)
    
    move $t3, $t2       # j = l

for_j:  bge $t3, $s0, end_j # if j >= n then endfor
    add $t4, $v1, $t3   # $t4 apunta a cad + j
    addi $t5, $t4, 1    # $t5 apunta a cad + j + 1
    lb $t6, 0($t5)      # $t6 = *(cad + j + 1)
    sb $t6, 0($t4)      # (cad + j) = *(cad + j + 1)
    addi $t3, $t3, 1    # j++
    j for_j
    


end_j:  add $t3, $v1, $s0   # $t3 = cad + n
    sb $0, 0($t3)

end_j2: addi $t0, $t0, 1    # contador++
    j for_i


end_i:  lw $ra, 0($sp)
    addi $sp, $sp, 4
    jr $ra


salir:  
    li $v0, 10
    syscall