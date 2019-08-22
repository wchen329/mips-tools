# Big Stall Forward
# This test case engages multiple forwarding paths and stalls
# It's a test for correctness
addi $s0, $zero, -1
sw $s0, $s0(3000)
lw $s1, $s0(3000)
sw $s1, $s1(4000)
lhu $s2, $s0(3000)
lhu $s3, $s1(4000)
beq $s2, $s3, SUCCESS
sll $s0, $s0, 0
sll $s0, $s0, 0
sll $s0, $s0, 0
sll $s0, $s0, 0
.exit
sll $s0, $s0, 0

SUCCESS:
addi $k0, $zero, 999
sll $s0, $s0, 0
sll $s0, $s0, 0
sll $s0, $s0, 0
sll $s0, $s0, 0
.exit
sll $s0, $s0, 0
