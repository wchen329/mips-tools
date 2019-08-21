addi $s0, $zero, 2000
sw $s0, $s0(4)
lw $s1, $s0(4)
beq $s0, $s1, SUCCESS
addi $k0, $zero, 15
sll $zero, $zero, 0
sll $zero, $zero, 0
sll $zero, $zero, 0
.exit
sll $zero, $zero, 0

SUCCESS:
	addi $k1, $zero, 15 
	sll $zero, $zero, 0
	sll $zero, $zero, 0
	sll $zero, $zero, 0
	.exit
	sll $zero, $zero, 0
