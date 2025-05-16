/* 
 * File:   C_code.c
 * Author: Chris
 *
 * Created on July 2, 2024, 12:27 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "config.h"

#define CLED PORTCbits.RC0
#define DLED PORTDbits.RD0
#define CSWITCH PORTCbits.RC1
#define DSWITCH PORTDbits.RD1

int main() 
{
    ADCON1 = 0xff;
    TRISC = 0xfe;
    TRISD = 0xfe;
    while (1)
    {
        CLED = CSWITCH;
        DLED = DSWITCH;
    }
    return (EXIT_SUCCESS);
}

