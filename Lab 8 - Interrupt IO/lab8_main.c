#include <stdio.h>
#include <stdlib.h>
#include "config.h"

#define input   PORTBbits.RB1
#define output  PORTDbits.RD1

void __interrupt() toggleLed(void);

int main() 
{
    ADCON1 = 0x0F;// All AN inputs are digital
    TRISD = 0x00; // PORTD is Output
    TRISB = 0xFF; // PORTB is Input 
    
    // Setup all inerrupts
    INTCONbits.GIE = 1; // Globabl interrupt enable
    INTCON3bits.INT1IE = 1; // Enable INT1
    INTCON3bits.INT1IF = 0; // reset INT1 flag
    
    while(1)
    {
    //Other processes would go here
    }
    return (EXIT_SUCCESS);
}

void __interrupt() toggleLed(void)
{
   if (INTCON3bits.INT1IE && INTCON3bits.INT1IF)
    {// Turn off the interrupt flag to avoid recursive interrupt calls
        INTCON3bits.INT1IF = 0;
        output = input;
    }
}