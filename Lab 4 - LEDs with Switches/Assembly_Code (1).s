config OSC = INTIO2
config BOR = OFF        
config STVREN = OFF    
config WDT = OFF        
config MCLRE = ON   
#include <xc.inc>	
	psect code
	goto Start
    
Start:
    setf    ADCON1, a   
    setf    TRISC, a    
    bcf    TRISC, 0, a  
    setf    TRISD, a    
    bcf    TRISD, 0, a 

    
Infloop:
    rrcf    PORTC, f, a
    rrcf    PORTD, f, a
    bra Infloop
    
end

