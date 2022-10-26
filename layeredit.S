#include <asm_setup.S>
.text
.global CustomZOrder
CustomZOrder:
	stwu   sp, -0x20(sp)
	mflr   r0
	stw    r0, 0x24(sp)
	stw    r3, 0x1C(sp)
	lis    r3, CustomZValue@ha   
	lfs    fX, CustomZValue@l(r3)
	stfs   fX, 0x<something>(rY)      #    stfs   fX, 0x<something>(rY)
	lwz    r3, 0x1C(sp)
	lwz    r0, 0x24(sp)
	mtlr   r0
	addi   sp, sp, 0x20
	blr

.data
CustomZValue:
	.float    5000.0    # Background: -5500.0
						# Layer 2: -3300.0
						# Layer 1: 0.0
						# Player: 300.0
						# Foreground: 1200.0
						# Layer 0: 3300.0
						# Above layer 0: 5500.0