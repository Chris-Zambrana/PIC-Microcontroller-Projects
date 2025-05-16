#include <xc.inc>
    psect code
    num_AH equ 0x20
    num_AM equ 0x21
    num_AL equ 0x22
    num_BH equ 0x50
    num_BM equ 0x51
    num_BL equ 0x52
    
    ; A=0xF191B5 AH=F1 AM=91 AL=B5
    movlw 0xF1
    movwf num_AH, 0
    movlw 0x91
    movwf num_AM, 0   
    movlw 0xB5
    movwf num_AL, 0 
    
    ;B=0x07A204 BH=07 BM=A2 BL=04
    movlw 0x07
    movwf num_BH, 0 
    movlw 0xA2
    movwf num_BM, 0   
    movlw 0x04
    movwf num_BL, 0    
    
    clrf WREG, 0
    
    ;add lower bytes
    movf num_AL, 0, 0
    addwf num_BL, 1, 0
    
    ;add middle bytes
    movf num_AM, 0, 0
    addwfc num_BM, 1, 0

    ;add high bytes
    movf num_AH, 0, 0
    addwfc num_BH, 1, 0

end