# Mem to Mem
# This test case engages multiple forwarding paths and stalls
# No stalls are expected.
addi $v0, $zero, -3000
sw $v0, $zero(9000)
lw $v1, $zero(9000)
sw $v1, $zero(10000)
lw $s0, $zero(10000)
sll $zero, $zero, 0
sll $zero, $zero, 0
sll $zero, $zero, 0
sll $zero, $zero, 0
.exit
