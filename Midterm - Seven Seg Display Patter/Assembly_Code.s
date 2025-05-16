config OSC = INTIO2
config BOR = OFF        ; Brown-out Reset Enable bits (Brown-out Reset disabled in hardware and software)
config STVREN = OFF     ; Stack Full/Underflow Reset Enable bit (Stack full/underflow will not cause Reset)
config WDT = OFF        ; Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
config MCLRE = ON       ; MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)
#include <xc.inc>	
	psect code
	goto Start	
	
	psect data
Lookup: DB 0xCF, 0xE7, 0xF3, 0xF9, 0xBD, 0x9F

SSEGend	EQU 0x46
SSEGbegin EQU 0x41    ; 7segment pattern table starting address in data memory
I	EQU 0x70    ; used as a counter index
Start:	
	; Move the 7seg pattern from program memory into data memory
	movlw	low Lookup
	movwf	TBLPTRL, 0
	movlw	high Lookup
	movwf	TBLPTRH, 0
	movlw	low highword Lookup
	movwf	TBLPTRU, 0
	
	lfsr	0, SSEGbegin ; starting address in data memory
	movlw	6
	movwf	I, 0 ; initialize counter with 16
	
Loop:	TBLRD*+    ; read 1B from program memory and advance TBLPTR by 1
	movff	TABLAT, POSTINC0 ;copy TABLAT into INDF0 them move FSR0 pointer forward
	decf	I, 1, 0;
	bnz	Loop
	
	setf ADCON1, 0   
	clrf TRISA, 0    
	setf TRISD, 0    
	
    
Infloop:
    call Delay500ms
    movf PORTD, 0, 0
    andlw 0x03
    bz Off
    decf WREG, 1, 0
    bz Togglenegative
    decf WREG, 1, 0
    bz Counterclockwise
    decf WREG, 1, 0
    bz Clockwise
    setf PORTA, 0
    bra Infloop

Off:
    setf PORTA, 0 
    bra Infloop
    
Togglenegative:
    setf PORTA, 0
    call Delay500ms
    movlw 0xfe
    movwf PORTA, 0
    bra Infloop
    
Clockwise:
    movf INDF0, 0, 0
    andlw 0xCF
    bz zeroTOsix
    movff POSTDEC0, PORTA
    bra Infloop
    
Counterclockwise:
    movf INDF0, 0, 0
    andlw 0x9F
    bz sixTOzero
    movff POSTINC0, PORTA
    bra Infloop
    
zeroTOsix:
    lfsr 0, SSEGend
    bra Clockwise
    
sixTOzero:
    lfsr 0, SSEGbegin
    bra Counterclockwise
    
Delay2550us:			    ; 2550 us delay
	movlw	255
l1:	decf	WREG, 0, 0
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	bnz	l1
	return 1

Delay500ms:			    
	movlw	55		    
	movwf	0x10, 0
l2:	call	Delay2550us
	decf	0x10, 1, 0
	bnz	l2
	return 1
end

