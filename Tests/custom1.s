# f 0 = 2
# | f n = 3 * f (n-1)

.data

out1:   .asciiz "Ingresar cantidad de numeros: "
out2:   .asciiz "Ingresar numero: "
coma:   .asciiz ", "

.text



main:       jal entrada



res_list:   beq $t2, $0, salir      # if j = 0 then exit
            lw $a0, 0($v1)          # else resolver. Cargar numero de la lista en variable
            jal tres

            move $a0, $v0
            li $v0, 1
            syscall

            li $v0, 4
            la $a0, coma
            syscall

            addi $v1, $v1, 4
            addi $t2, $t2, -1
            j res_list



entrada:    li $v0, 4
            la $a0, out1    # Mostrar "Ingrese cantidad de numeros"
            syscall

            li $v0, 5
            syscall

            move $t0, $v0   # Variable para recorrer ingreso lista
            move $t2, $v0   # Variable para recorrer lista a resolver

            li $a0, 4
            li $v0, 9       # Primera direccion
            syscall

            move $v1, $v0   # Guarda direccion inicial de la lista
            move $t1, $v0   # Puntero para ingreso de lista


ing_list:   beq $t0, $0, exit_ing   #if i = 0 then salir
            li $v0, 4               # else ingresar numero
            la $a0, out2            # Mostrar "Ingrese numero"
            syscall

            li $v0, 5               # Llamada al sistema que permite ingresar numero
            syscall

            sw $v0, 0($t1)          # Guardar numero en lista

            li $a0, 4
            li $v0, 9               # Solicito espacio para el bloque siguiente en lista
            syscall 

            move $t1, $v0
            addi $t0, $t0, -1
            j ing_list


exit_ing:   jr $ra


tres:       bgt $a0, $0, rec
            li $v0, 2
            jr $ra


rec:        addi $sp, $sp, -4
            sw $ra, 0($sp)

            addi $a0, $a0, -1
            jal tres

            li $a1, 3
            mul $v0,$v0,$a1
            lw $ra, 0($sp)
            addi $sp, $sp, 4
            jr $ra


salir:      li $v0, 10
            syscall