.data
a: .word 10
b: .word 10
sum: .word 0

perrito: .asciiz "hi folks"

.text
.globl main
main:
	lw $t0, a
	lw $t1, b
	add $t0, $t1, $t2
	sw $t2, sum
.end main
