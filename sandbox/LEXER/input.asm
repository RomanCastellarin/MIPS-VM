.data
a: .word 10 #una palabra
b: .word 10 #otra
sum: .word 0

perrito: .asciiz "hi folks"
pepe: .asciiz "sup boys"

.text
.globl main #remember
main:
	lw $t0, a
	lw $t1, b
	add $v0, $t1, $k0
    sub $t0, $t1, $t2
    addi $t1, $t2, 18
    sll $t1, $t2, 65
	sw $t2, sum
.end main
