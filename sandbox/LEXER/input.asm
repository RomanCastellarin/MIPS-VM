.data
var1: .word 30 
sum: .word 1

perrito: .asciiz "hi foasdsadsadsadsadasdsadlks"
pepe: .asciiz "sup boys"

.text
.globl main #remember
main:
	lw $t0, var1
	sw $t2, sum
.end main
