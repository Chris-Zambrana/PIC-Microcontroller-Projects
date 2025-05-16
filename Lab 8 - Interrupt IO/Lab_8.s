config OSC = INTIO2
config BOR = OFF        ; Brown-out Reset Enable bits (Brown-out Reset disabled in hardware and software)
config STVREN = OFF     ; Stack Full/Underflow Reset Enable bit (Stack full/underflow will not cause Reset)
config WDT = OFF        ; Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
config MCLRE = ON       ; MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)
#include <xc.inc>
    goto main	
    
    psect intCodeHi, class = CODE, reloc = 2
    ; ISR
    bcf	    INTCON3, 0, a
    movff   PORTB, PORTD
    retfie  0
    
    psect code
main:
    ; set the I/O port directions
    setf    ADCON1, a   ; AN inputs are now digital inputs
    clrf    TRISD, a	; Sets RD0 as outputs (connected to LED)
    setf    TRISB, a	; RB0 is input (connected to LM339 output)
     
    ; setup interupt
    bsf	    INTCON, 7, a  ; Enables Global Interrupt
    movlw   01001000B	  ; Enable and disable for INTCON3
    movwf   INTCON3, f, a ; INT1IP set to high & INT1IE enabled
    
loop:
    ;Other processes would go here
    bra	    loop
    
end