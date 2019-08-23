# Store Close
# Tests several adjacent store operations
addi $t0, $zero, 1
addi $t1, $zero, 2 
addi $t2, $zero, 3
addi $t3, $zero, 4
addi $t4, $zero, 5
addi $t5, $zero, 6

# Store these three using store operations
sb $t0, $zero(900)
sb $t1, $zero(901)
sh $t2, $zero(902)
sh $t3, $zero(904)
sw $t4, $zero(906)
sb $t5, $zero(910)

# Unload
lbu $s0, $zero(900)
lbu $s1, $zero(901)
lhu $s2, $zero(902)
lhu $s3, $zero(904)
lw $s4, $zero(906)
lw $s5, $zero(910)

sll $zero, $zero, 0
sll $zero, $zero, 0
sll $zero, $zero, 0
sll $zero, $zero, 0
.exit
