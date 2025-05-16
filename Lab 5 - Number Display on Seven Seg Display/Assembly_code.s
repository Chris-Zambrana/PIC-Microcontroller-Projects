config OSC = INTIO2
config BOR = OFF        ; Brown-out Reset Enable bits (Brown-out Reset disabled in hardware and software)
config STVREN = OFF     ; Stack Full/Underflow Reset Enable bit (Stack full/underflow will not cause Reset)
config WDT = OFF        ; Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
config MCLRE = ON       ; MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)
#include <xc.inc>	
	psect code
	goto Start	
	
	; Convert BCD into 7 Segment pattern
	; Start by copying the program from subroutine
	
	psect data  
Lookup:	DB  0x81, 0xCF, 0x92, 0x86, 0xCC, 0xA4, 0xA0, 0x8F, 0x80, 0x84, 0x88, 0xE0, 0xB1, 0xC2, 0xB0, 0xB8
	
SSEG	EQU 0x41    ; 7segment pattern table starting address in data memory
I	EQU 0x70    ; used as a counter index
Start:	
	; Move the 7seg pattern from program memory into data memory
	movlw	low Lookup
	movwf	TBLPTRL, 0
	movlw	high Lookup
	movwf	TBLPTRH, 0
	movlw	low highword Lookup
	movwf	TBLPTRU, 0
	
	lfsr	0, SSEG ; starting address in data memory
	movlw	16
	movwf	I, 0 ; initialize counter with 16
Loop:	TBLRD*+    ; read 1B from program memory and advance TBLPTR by 1
	movff	TABLAT, POSTINC0 ;copy TABLAT into INDF0 them move FSR0 pointer forward
	decf	I, 1, 0;
	bnz	Loop
	
	; set the I/O port directions
	setf	ADCON1, 0   ; turn off the ADC
	clrf	TRISA, 0    ; output connected to 7seg
	setf	TRISB, 0    ; input  connected to 4 switches
	
	; start the read/display
Infiniteloop:
	movf	PORTB, 0, 0 ; read value from switches
	andlw	0x0f	    ; keep the lowest 4 bits only
	call	Bcd2sseg, 0
	movwf	PORTA, 0
	bra	Infiniteloop
    
	; convert a BCD pattern stored in 4 lsb of WREG into 7Seg
Bcd2sseg: 
	lfsr	0, SSEG; move fsr0 pointer back to start of table
	movf	PLUSW0, 0, 0
	return 0; WREG will have the sseg pattern upon return
end
