#include <xc.inc>
	goto start	
	; Lab 3 Prelab
	psect code
N   equ	0x00	; Number of X's
Z   equ 0x60	; Final answer is stored here
X   equ 0x50	; Starting point of Xi array

start:	; load location 0x50 to 0x57 with arbitrary numbers for testing
	; you should also use "Memory Fill" to test it with zeros, ones,
	; or even random numbers
	lfsr	0, X
	movlw	1
	movwf	POSTINC0, a
	movlw	2
	movwf	POSTINC0, a
	movlw	3
	movwf	POSTINC0, a
	movlw	4
	movwf	POSTINC0, a
	movlw	5
	movwf	POSTINC0, a
	movlw	6
	movwf	POSTINC0, a
	movlw	7
	movwf	POSTINC0, a
	movlw	8
	movwf	POSTINC0, a
	
    	; TODO: Your main program code starts here
	; 	- Use FSR1 to point to Xi
	;	- Call sum8 to generate the sum
	movlw	8
	movwf	N, f, a
	clrf	Z, a; Z = 0
	lfsr	0, X
	lfsr	1, N
	call	sum8, 1
	;	- Divide the resulting sum by 8 using rotate right with carry 
	;	  (you should reset the carry after each rotation
	rrcf	Z, f, a ; Divide by 2
	bcf	Z, 7, a ; Clear Most significant bit
	
	rrcf	Z, f, a ; Divide by 2
	bcf	Z, 7, a ; Clear Most significant bit
	
	rrcf	Z, f, a ; Divide by 2
	bcf	Z, 7, a ; Clear Most significant bit
	
stop:	bra	stop

sum8:	; TODO: Your subroutine program codes goes here
	movff	POSTINC0, WREG, a
	addwf	Z, w, a
	movwf	Z, f, a
	decfsz	N, f, a
	bnz	sum8
	return 1
	
end