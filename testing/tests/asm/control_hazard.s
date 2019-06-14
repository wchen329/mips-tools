addi $s0, $s0, 100
.main:
addi $s1, $s1, 50
.state
bne $s0, $s1, main
sll $zero, $zero, 0
sll $zero, $zero, 0
.exit
