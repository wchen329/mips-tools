# Fake Load to Use
# Load to use to an instruction writing to the zero register
# No stalls expected
lw $zero, $zero(5000)
add $zero, $zero, $zero
sll $zero, $zero, 0
sll $zero, $zero, 0
sll $zero, $zero, 0
sll $zero, $zero, 0
.exit
