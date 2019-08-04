addi $t1, $t0, 400
beq $t1, $zero, NODICE 

SUCCESS:
	sll $s0, $s0, 0
	sll $s0, $s0, 0
	sll $s0, $s0, 0
	addi $t2, $zero, -100
	sll $s0, $s0, 0
NODICE:
	sll $s0, $s0, 0
	sll $s0, $s0, 0
	sll $s0, $s0, 0
	sll $s0, $s0, 0
	.state
	.exit
