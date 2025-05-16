/* 
 * File:   C_code.c
 * Author: Chris
 *
 * Created on July 5, 2024, 1:44 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "config.h"

int main() 
{
    char sseg[15] = {0x81, 0xCF, 0x92, 0x86, 0xCC, 0xA4, 0xA0, 0x8F, 0x80, 0x84, 0x88, 0xE0, 0xB1, 0xC2, 0xB0};
    
    ADCON1 = 0xff;
    TRISC = 0x00;
    TRISD = 0xff;
    
    int sum;
    while(1)
    {
        int num1, n2, num2;
        num1 = (PORTD & 0x07);
        n2 = (PORTD & 0x38);
        num2 = (n2 >> 3);
        sum = num1 + num2;
        PORTC = sseg[sum & 0x0f];
    }
    return (EXIT_SUCCESS);
}

