main:
	addi $s0, $zero, 100 # Not a no-op
.state
	sll $zero, $zero, 0 # No-op!
.exit
