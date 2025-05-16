#include <stdio.h>
#include <stdlib.h>
#include "config.h"

#define input   PORTBbits.RB1
#define output  PORTDbits.RD1
int main() 
{
    ADCON1 = 0x0F;// All AN inputs are digital
    TRISD = 0x00; // PORTD is Output
    TRISB = 0xFF; // PORTB is Input 
    while(1)
    {    
        output = input;
    }
    return (EXIT_SUCCESS);
}