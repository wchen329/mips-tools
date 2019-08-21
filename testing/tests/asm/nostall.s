# "No stall"
# This test cases tries to catch several instructions that
# may accidentally cause stalls but do not require them

# Load to use, separated by no-op
addi $v0, $zero, 700
sw $v0, $zero(700)
addi $s0, $zero, 500
sw $s0, $zero(200)
lw $s3, $zero(200)
sll $zero, $zero, 0
lw $s1, $s3(200)

# Stores followed directly by a load
addi $a0, $zero, 15

sb $a0, $a0(1000)
lbu $t0, $a0(1000)
sh $a0, $a0(3000)
lhu $t1, $a0(3000)
sw $a0, $a0(5000)
lw $t2, $a0(5000)

sll $zero, $zero, 0
sll $zero, $zero, 0
sll $zero, $zero, 0
sll $zero, $zero, 0
sll $zero, $zero, 0
.exit
