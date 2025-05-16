/* 
 * File:   C_code.c
 * Author: Chris
 *
 * Created on July 2, 2024, 8:36 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "config.h"

#define _XTAL_FREQ 1000000
#define SWITCH1 PORTDbits.RD1
#define SWITCH2 PORTDbits.RD0


int main() 
{
    char sseg[6] = {0xCF, 0xE7, 0xF3, 0xF9, 0xBD, 0x9F};
    ADCON1 = 0xff;
    TRISA = 0x00;
    TRISD = 0xff;
    
    char c1 = 0;
    while(1)
    {
        if (SWITCH1 == 1 && SWITCH2 == 1)
        {
            __delay_ms(500);
            PORTA=sseg[c1];
            if (c1 == 0)
            {
              c1 = 5;
            }
            else
            {
                c1 = (c1 - 1);
            }
        }
        else if(SWITCH1 == 0 && SWITCH2 == 1)
        {
            __delay_ms(500);
            PORTA = 0xff;
            __delay_ms(500);
            PORTA = 0xfe;

        }
        else if(SWITCH1 == 1 && SWITCH2 == 0)
        {
            __delay_ms(500);
            PORTA=sseg[c1];
            c1 = (c1 + 1) % 6;
        }
        else
        {
            PORTA = 0xff;
        }
    }

}

