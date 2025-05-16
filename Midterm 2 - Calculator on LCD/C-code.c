/* 
 * File:   C-code.c
 * Author: Chris
 *
 * Created on July 22, 2024, 2:43 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "LiquidCrystal.h"
#include "config.h"

#define _XTAL_FREQ 1000000 
#define SW1 PORTDbits.RD4
#define SW2 PORTDbits.RD3

void __interrupt() adc(void);

volatile signed long pot1;
volatile signed long pot2;
volatile signed long num;

int main() 
{
      // Configure LCD Pins
    // Data pins connected to PORTB
    TRISB = 0x00; 
    
    // RS = RD0
    // RW = RD1
    // E  = RD2
    // Switches = D3 & D4
    TRISD = 0x18;
    
//    INTCON = 0x00;
    // connect the LCD pins to the appropriate PORT pins
    pin_setup(&PORTB, &PORTD);
    
    // initialize the LCD to be 16x2 (this is what I have, yours might be different)
    begin(16, 2, LCD_5x8DOTS);
    
    
    //--------------------------------------------------------------------------
    // 1 - Configure the A/D Module

    // * Configure analog pins, voltage reference and digital I/O 
    // Reference voltages are VSS and VDD
    ADCON1 = 0x0D;
    TRISAbits.RA0 = 1; // connected to potentiometer 1
    TRISAbits.RA1 = 1; // connected to potentiometer 2

    // * Select A/D acquisition time
    // * Select A/D conversion clock
    // Right justified, ACQT = 2 TAD, ADCS = FOSC/2
    ADCON2bits.ADCS = 0; // FOSC/2
    ADCON2bits.ACQT = 1; // ACQT = 2 TAD
    ADCON2bits.ADFM = 1; // Right justified

    // * Select A/D input channel
    ADCON0bits.CHS = 0; // Channel 0 (AN0), starting with the potentiometer

    // * Turn on A/D module
    ADCON0bits.ADON = 1;   
    
    // 2 - Configure A/D interrupt (if desired)
    // * Clear ADIF bit
    // * Set ADIE bit
    // * Select interrupt priority ADIP bit
    // * Set GIE bit
    
    PIR1bits.ADIF = 0;
    PIE1bits.ADIE = 1;
    IPR1bits.ADIP = 1;
    RCONbits.IPEN = 0; // disable priority levels
    INTCONbits.PEIE = 1; // enable peripheral interrupts
    INTCONbits.GIE = 1;
    
    // 3 - Wait the required acquisition time (if required)
    // ---> using ACQT = 2 TAD, no need to manually specify a wait    
    clear();
    
    while(1)
    {
        ADCON0bits.GO = 1;   
        
        home();
        
        if (SW1 == 1 && SW2 == 1) // Multiply
        {
            displayON();
            num = pot1 * pot2;
            print("(");
            print_int(pot1);
            print(")");
            print("x(");
            print_int(pot2);
            print(")=");
            print("               ");
            setCursor(0,1);
            print_int(num);
            print("               ");
        }
        else if(SW1 == 1 && SW2 == 0) // Subtract
        {
            displayON();
            num = pot1 - pot2;
            print("(");
            print_int(pot1);
            print(")");
            print("-(");
            print_int(pot2);
            print(")=");
            print("               ");
            setCursor(0,1);
            print_int(num);
            print("               ");
        }
        else if(SW1 == 0 && SW2 == 1) // Add
        {
            displayON();
            num = pot1 + pot2;
            print("(");
            print_int(pot1);
            print(")");
            print("+(");
            print_int(pot2);
            print(")=");
            print("               ");
            setCursor(0,1);
            print_int(num);
            print("               ");
        }
        else // Display OFF
        {
            displayOFF();
        }
        
    }

    return (EXIT_SUCCESS);
}

void __interrupt() adc(void)
{
    // test which interrupt called this interrupt service routine
    
    // ADC Interrupt
    if (PIR1bits.ADIF && PIE1bits.ADIE)
    {
        // 5 Wait for A/D conversion to complete by either
        // * Polling for the GO/Done bit to be cleared
        // * Waiting for the A/D interrupt
  
        // 6 - Read A/D result registers (ADRESH:ADRESL); clear bit ADIF, if required
        
        // reset the flag to avoid recursive interrupt calls
        PIR1bits.ADIF = 0;
        
        if (ADCON0bits.CHS == 0) // channel AN0 (potentiometer 1)
        {
            pot1 = (ADRESH << 8) | ADRESL;
            pot1 = pot1 - 511;
            ADCON0bits.CHS = 1;
        }
        else if (ADCON0bits.CHS == 1) // channel AN1 (potentiometer 2)
        {
            pot2 = (ADRESH << 8) | ADRESL;
            pot2 = pot2 - 511;
            ADCON0bits.CHS = 0;
        }
    }
}
