config OSC = INTIO2
config BOR = OFF        ; Brown-out Reset Enable bits (Brown-out Reset disabled in hardware and software)
config STVREN = OFF     ; Stack Full/Underflow Reset Enable bit (Stack full/underflow will not cause Reset)
config WDT = OFF        ; Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
config MCLRE = ON       ; MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)
#include <xc.inc>	
	psect code
	goto start	
	; Convert BCD into 7 Segment pattern
	; Start by copying the program from subroutine
	
	psect data
lookup:	DB  0x81, 0xCF, 0x92, 0x86, 0xCC, 0xA4, 0xA0, 0x8F, 0x80, 0x84, 0x88, 0xE0, 0xB1, 0xC2, 0xB0, 0xFF
	
NUMA EQU 0x50
NUMB EQU 0x51
SSEG	EQU 0x40    ; 7segment pattern table starting address in data memory
I	EQU 0x70    ; used as a counter index
start:	
	; Move the 7seg pattern from program memory into data memory
	movlw	low lookup
	movwf	TBLPTRL, a
	movlw	high lookup
	movwf	TBLPTRH, a
	movlw	low highword lookup
	movwf	TBLPTRU, a
	
	lfsr	0, SSEG ; starting address in data memory
	movlw	16
	movwf	I, a ; initialize counter with 16
loop:	TBLRD*+    ; read 1B from program memory and advance TBLPTR by 1
	movff	TABLAT, POSTINC0 ;copy TABLAT into INDF0 them move FSR0 pointer forward
	decf	I, f, a;
	bnz	loop
	
	; set the I/O port directions
	setf	ADCON1, a   ; turn off the ADC
	clrf	TRISC, a    ; output connected to 7seg
	setf	TRISD, a    ; input  connected to 4 switches
	
	; start the read/display
infiniteloop:
	movf	PORTD, w, a ; read value from switches
	andlw	0x07	    ; keep the lowest 4 bits only
	movwf	NUMA, a
	movf	PORTD, w, a
	andlw	0x38
	movwf	NUMB, a
	rrncf	NUMB, f, a
	rrncf	NUMB, f, a
	rrncf	NUMB, f, a
	movf	NUMA, w, a
	addwf	NUMB, w, a 
	call	bcd2sseg, 0
	movwf	PORTC, a
	bra	infiniteloop
    
	; convert a BCD pattern stored in 4 lsb of WREG into 7Seg
bcd2sseg: 
	lfsr	0, SSEG; move fsr0 pointer back to start of table
	movf	PLUSW0, w, a
	return 0; WREG will have the sseg pattern upon return
end


