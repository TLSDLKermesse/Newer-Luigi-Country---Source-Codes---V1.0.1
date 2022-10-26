.text

#include <asm_setup.S>

.align 4
.extern slowDownTime
.extern updateUI
.extern ApparentlyStartsMusic
.extern continueTimerLeq100
.extern continueAt8019e154
.extern continueAt8019e140
.extern getTimeSfxIndex__Fv


.global TimeCmpwi100
TimeCmpwi100:								
	cmpwi r0, 100
	bgt		TimeIsBgt100				#if time is greater than 100 branch to TimeIsBgt100
	b		continueTimerLeq100			#else return to orginal code for is less or equals 100


.global TimeIsBgt100
TimeIsBgt100:
	lis		r4, slowDownTime@h			#Store slowDownTime address in r4
	ori		r4, r4, slowDownTime@l
	lbz		r0, 0x0(r4)					#Store slowDownTime value in r0
	cmpwi	r0, 0x1						#If slowDownTime = True
	beq		slowDownMusic				#Slow down the music
	b		updateUI					#return to original code


.global slowDownMusic
slowDownMusic:
	li		r0, 0x0						#Load false in r0
	stb		r0, 0xb(r31)				#Store r0 in paused
	li 		r4, 0x10
	lwz		r3, -0x51f8(r13)
	bl		ApparentlyStartsMusic		#start slowed down music
	b		updateUI					#return to original code



.global speedSelection
speedSelection:
	cmpwi	r0, 0x0
	beq		continueForNormalSpeed
	lis		r3, slowDownTime@h			#Store slowDownTime address in r3
	ori		r3, r3, slowDownTime@l
	lbz		r0, 0x0(r3)					#Store slowDownTime value in r0
	cmpwi	r0, 0x1						#if slowDownTime is true ...
	li		r0, 0x0						
	stb		r0, 0x0(r3)					#Store false at r3 aka slowDownTime
	beq		continueForNormalSpeed		#... goto continueForNormalSpeed
	b		continueAt8019e140


.global continueForNormalSpeed			#roundabout return to original code because Kamek is bad!
continueForNormalSpeed:
	b		continueAt8019e154



.global selectSfx
selectSfx:
	stwu sp, -0x40(sp)
	stw r0, 0x44(sp)
	mflr r0
	stw r0, 0x3C(sp)
	stw r3, 0x38(sp)
	stw r5, 0x34(sp)
	stw r6, 0x30(sp)
	stw r24, 0x2C(sp)
	stw r25, 0x28(sp)
	stw r26, 0x24(sp)
	stw r27, 0x20(sp)
	stw r28, 0x1C(sp)
	stw r29, 0x18(sp)
	stw r30, 0x14(sp)
	stw r31, 0x10(sp)
	
	
	li		r5, 0
	
	lis		r4, slowDownTime@h			#Store slowDownTime address in r4
	ori		r4, r4, slowDownTime@l
	lbz		r0, 0x0(r4)					#Store slowDownTime value in r0
	cmpwi	r0, 0x1						#If slowDownTime = True
	#li		r0, 0x0						
	#stb		r0, 0x0(r4)					#Store false at r4 aka slowDownTime
	beq		slowDownSfx
	b		hurryUpSfx



.global hurryUpSfx
hurryUpSfx:
	bl		getTimeSfxIndex__Fv			#get sfx index in r3

	andi.	r3, r3, 0xF					#only leave right nybble, index!
	mulli	r3, r3, 0x2					#each entry is 2 bytes ...

	lis r4, hurryUpSfxList@h			#get sfx list
	ori r4, r4, hurryUpSfxList@l		#get sfx list
	add r3, r3, r4						#add selected index

	lhz r4, 0(r3)

	b		sfx100Return


.global slowDownSfx
slowDownSfx:
	bl		getTimeSfxIndex__Fv			#get sfx index in r3

	srwi	r3, r3, 4					#>>4
	andi.	r3, r3, 0xF					#only leave left nybble, index!
	mulli	r3, r3, 0x2					#each entry is 2 bytes ...

	lis r4, slowDownSfxList@h			#get sfx list
	ori r4, r4, slowDownSfxList@l		#get sfx list
	add r3, r3, r4						#add selected index

	lhz r4, 0(r3)

	b		sfx100Return



.global sfx100Return
sfx100Return:
	lwz r31, 0x10(sp)
	lwz r30, 0x14(sp)
	lwz r29, 0x18(sp)
	lwz r28, 0x1C(sp)
	lwz r27, 0x20(sp)
	lwz r26, 0x24(sp)
	lwz r25, 0x28(sp)
	lwz r24, 0x2C(sp)
	lwz r6, 0x30(sp)
	lwz r5, 0x34(sp)
	lwz r3, 0x38(sp)
	lwz r0, 0x3C(sp)
	mtlr	r0
	lwz r0, 0x44(sp)
	addi	sp, sp, 0x40
	blr




.align 4
.data
.global hurryUpSfxList
.global slowDownSfxList		#0,       1,      2,      3,      4,      5,      6,      7,      8,      9,     10,     11,     12,     13,     14,     15
hurryUpSfxList:		.short 0x00AF, 0x00AF, 0x00AF, 0x00AF, 0x0500, 0x00AF, 0x00AF, 0x00AF, 0x00AF, 0x00AF, 0x00AF, 0x00AF, 0x00AF, 0x00AF, 0x00AF, 0x00AF
slowDownSfxList:	.short 0x0500, 0x00AF, 0x00AF, 0x00AF, 0x00AF, 0x00AF, 0x00AF, 0x00AF, 0x00AF, 0x00AF, 0x00AF, 0x00AF, 0x00AF, 0x00AF, 0x00AF, 0x00AF