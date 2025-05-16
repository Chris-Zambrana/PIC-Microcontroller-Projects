#include <xc.inc>
    psect code
    /*It is desired to add two 24-bit data items in memory as shown 
     in the figure below. Store the result pointed to by 0x50. The 
     operation with sample data is given by 
	  F1 91 B5 
     PLUS 07 A2 04
     L being lower bit, M being the middle bit, & H being higher bit*/
    num_AL equ 0x30
    num_AM equ 0x31
    num_AH equ 0x32
 
    num_BL equ 0x40
    num_BM equ 0x41
    num_BH equ 0x40
 
    num_CL equ 0x50
    num_CM equ 0x51
    num_CH equ 0x52
    
    ;A = 0xF191B5
    movlw 0xB5
    movwf num_AL, f, a
    movlw 0x91
    movwf num_AM, f, a
    movlw 0xF1
    movwf num_AH, f, a
    
    ;B = 0x07A204
    movlw 0x04
    movwf num_BL, f, a
    movlw 0xA2
    movwf num_BM, f, a
    movlw 0x07
    movwf num_BH, f, a
    
    clrf  WREG, a;Clears the working register
    
    ;Sum the lower bytes
    movf num_AL, w, a
    addwfc num_BL, w, a
    ;This might result in a carry so we need to capture it for the middle bytes
    movwf num_CL, f, a
    
    ;Sum the middle bytes
    movf num_AM, w, a
    addwfc num_BM, w, a; adding with the carry
    ;This might result in a carry so we need to capture it for the high bytes
    movwf num_CM, f, a
    
    ;Sum the middle bytes
    movf num_AH, w, a
    addwfc num_BH, w, a; adding with the carry
    
    movwf num_CH, f, a
    
end