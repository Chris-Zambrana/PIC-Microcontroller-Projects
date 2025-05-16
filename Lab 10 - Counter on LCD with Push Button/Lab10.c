#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "LiquidCrystal.h"

#define _XTAL_FREQ 1000000 
#define plus PORTBbits.RB0
#define minus PORTBbits.RB1
#define zero PORTBbits.RB2

int main() 
{
    
    // ALL ANx input are digital
    ADCON1 = 0x0f;
    
    // Data pins connected to PORTA
    TRISA = 0x00; 
    
    // PORTB is Input 
    TRISB = 0xFF; 
    
    // RS = RD0
    // RW = RD1
    // E  = RD2
    TRISD = 0x00;
    
    // connect the LCD pins to the appropriate PORT pins
    pin_setup(&PORTA, &PORTD);
    
    // initialize the LCD to be 16x2 (this is what I have, yours might be different)
    begin(16, 2, LCD_5x8DOTS);
    
    int num = 0;
    clear();
    while (1)
    {
        home();
        if(plus == 0)
        {
            clear();
            num = num + 1;
            print_int(num);
        }
        else if(minus == 0)
        {
            clear();
            num = num -1;
            print_int(num);
        }
        else if(zero == 0)
        {
            clear();
            num = 0;
            print_int(num);
        }
        else
        {
            //Not needed
        }
     
        __delay_ms(75);
    }
    
    return 0;
}
