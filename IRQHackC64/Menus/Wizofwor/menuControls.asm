;##############################################################################
; IRQHack64 Cartridge Main Menu - by wizofwor
; Menu Controls
;##############################################################################

!zone keyboardScan {
.KEY_MINUS = $2d ; -
.KEY_PLUS  = $2b ; +
.KEY_GT = $2e ; >
.KEY_LT = $2c ; <
.KEY_ENTER = $0d
.KEY_LEFT  = $9d
.KEY_RIGHT = $1d
.KEY_UP    = $91
.KEY_DOWN  = $11
.KEY_F1	   = 133
.KEY_F7	   = 136

.keyboardScan:

	; scan first joyport 2
	lda $DC00 ;joyport 2
	and #17
	beq .prevPage
	lda $DC00 ;joyport 2
	and #18
	beq .nextPage
	lda $DC00 ;joyport 2
	and #16
	beq j1
	lda $DC00 ;joyport 2
	and #2
	beq .downj
	lda $DC00 ;joyport 2
	and #1
	beq .upj

	jsr SCNKEY		; Call kernal's key scan routine
 	jsr GETIN		; Get the pressed key by the kernal routine
 	cmp #.KEY_MINUS	; IF char is '-'
 	beq .down 		; go down in menu
	cmp #.KEY_DOWN
	beq .down 		; go down in menu
 	cmp #.KEY_PLUS	; IF char is '+'
	beq .up 		; go up in menu
	cmp #.KEY_UP
	beq .up
 	cmp #.KEY_GT 	; IF char is '>'
 	beq .nextPage 	; request next page from micro
	cmp #.KEY_F7
	beq .nextPage 	; request next page from micro
 	cmp #.KEY_LT    ; IF char is '<'
 	beq .prevPage 	; request previous page from micro
	cmp #.KEY_F1
	beq .prevPage 	; request previous page from micro
 	cmp #.KEY_ENTER	; IF char is 'ENTER'
 	beq j1 			; launch selected item
	cmp #.KEY_RIGHT
	beq j1 			; launch selected item
    cmp #$0f 		; IF char is 'F'
    ;beq .simulation ; Display simulation menu
 	jmp .end

.nextPage:

	ldx PAGEINDEX
	inx
	cpx numberOfPages
	bcc .execNext	;BLT
	jmp .end

.prevPage

	ldx PAGEINDEX
	bne .execPrev
	jmp .end

j1: jmp enter

.downj
	ldx $D012
	cpx #$A0
	bne .downj

.down
	;clear old coloring
	ldy #22
	lda #$0f
-	sta (activeMenuItemAddr),y
	dey
	bne -

	;increment ACTIVE_ITEM
	ldx numberOfItems 	;check if the cursor is
	dex 				;already at end of page
	cpx activeMenuItem
	beq .end
	inc activeMenuItem

	clc
	lda activeMenuItemAddr
	adc #40
	sta activeMenuItemAddr
	lda activeMenuItemAddr+1
	adc #00
	sta activeMenuItemAddr+1

	jmp .end

.upj
	ldx $D012
	cpx #$A0
	bne .upj

.up

	;clear old coloring
	ldy #22
	lda #$0f
-	sta (activeMenuItemAddr),y
	dey
	bne -

	;decrement ACTIVE_ITEM
	lda #00 			;check if the cursor is
	cmp activeMenuItem 	;already at end of page
	beq .end
	dec activeMenuItem

	sec
	lda activeMenuItemAddr
	sbc #40
	sta activeMenuItemAddr
	lda activeMenuItemAddr+1
	sbc #00
	sta activeMenuItemAddr+1

	jmp .end

.execNext:

	inc PAGEINDEX
	ldx #COMMANDNEXTPAGE
	stx COMMANDBYTE
	jmp j1

.execPrev

	dec PAGEINDEX
	ldx #COMMANDPREVPAGE
	stx COMMANDBYTE
	jmp j1

.end

}

!zone colorwash {
	ldy #22
	lda #$07
-	sta (activeMenuItemAddr),y
	dey
	bne -
}
