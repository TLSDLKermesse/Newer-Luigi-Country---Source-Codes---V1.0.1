.text
#include <asm_setup.S>
.align 4

.global HigherAmountOfLives
HigherAmountOfLives:
	lbz r10, 0(r10)
	cmpwi r10, 0

	li r0, 0
	cmpwi r0, 0
	blr




.align 4

.data

#What is that language???