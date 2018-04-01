.data

out1:   .asciiz "Ingresar cantidad de fibonaccis: "
out2:   .asciiz "Ingresar orden de Fibonacci: "
coma:   .asciiz ", "


.text

main:       jal entrada

loop0:      beq $t2, $0, salir  # Salto a salir si no hay mas numeros para resolver en la lista
            lw $a0, 0($v1)      # Cargar el numero de donde estoy parado en la lista
            jal fib             # Salto a la subrutina que resuelve el problema
            move $a0, $v0       # Mover el resultado obtenido a $a0 para luego
            li $v0, 1           # mostrarlo
            syscall
            li $v0, 4           # Llamada al sistema para mostrar la coma
            la $a0, coma
            syscall     
            addi $v1, $v1, 4    # Ir al numero siguiente en lista
            addi $t2, $t2, -1   # Contador--
            j loop0             # Volver a iterar

entrada:    li $v0, 4           
            la $a0, out1
            syscall
            li $v0, 5           # Solicitar cantidad de numeros para la lista. Este se guardara en $v0
            syscall
            move $t0, $v0       # Copiar numero para variable temporal de la iteracion que ingresa la lista
            move $t2, $v0       # Idem pero para la iteracion que resuelve la lista
            li $a0, 4           # Pedir 4 bytes de espacio
            li $v0, 9           # mediante sbrk que representa un bloque de la lista, es decir, un numero
            syscall
            move $v1, $v0       # En $v1 se guarda la direccion del inicio de la lista
            move $t1, $v0       # Esta variable sera temporal para ingresar en lista y luego desplazar un lugar

loop1:      beq $t0, $0, endloop1   # Esto pregunta si ya no hay mas numeros para ingresar
            li $v0, 4           
            la $a0, out2
            syscall
            li $v0, 5           # Solicitar el numero a ingresar en lista
            syscall         
            sw $v0, 0($t1)      # Guardar el numero, ingresado en $v0, en la lista
            li $a0, 4           # Solicitar un bloque para la lista
            li $v0, 9
            syscall
            move $t1, $v0       # Se desplaza la variable temporal de la lista a la derecha
            addi $t0, $t0, -1   # Contador-- de la iteracion
            j loop1 

endloop1:   move $v0, $t2       # Se pasa el total de numeros a $v0. Posiblemente esta linea este de mas
            jr $ra              # Vuelta al inicio de la iteracion que resuelve los numeros


fib:        li $t0, 1
            bgt $a0, $t0, rec     # Si numero > 1, se realiza el proceso recursivo
            move $v0, $a0       # Sino se guarda en $v0 para iniciar el calculo
            jr $ra              # Si se ingreso 0 o 1, este salto es hacia la iteracion que muestra los numeros
                                # de lo contrario, es hacia la subrutina de recursividad

rec:        addi $sp, $sp, -12  # Se dejan 3 lugares. Uno para $ra, otro para el n temporal y otro para 1 o 0
            sw $ra, 0($sp)      # Guarda direccion de retorno
            sw $a0, 4($sp)      # Guarda el numero ingresado o restado 
            addi $a0, $a0, -1   # n--
            jal fib 
            
            sw $v0, 8($sp)      # Se guarda 0, 1, obtenido de arriba o el numero obtenido abajo
            lw $a0, 4($sp)      # Cargar el n afectado para continuar con la recursividad
            addi $a0, $a0, -2   # Ahora se hace fib (n-2)
            jal fib
            
            lw $a1, 8($sp)      # Se carga el numero a suma
            add $v0, $v0, $a1   # Suma entre el obtenido en fib o en rec y el numero recien cargado de pila
            lw $ra, 0($sp)      # Carga de direccion de retorno 
            addi $sp, $sp, 12   # Push
            jr $ra


salir:      li $v0, 10          # Bye
            syscall 