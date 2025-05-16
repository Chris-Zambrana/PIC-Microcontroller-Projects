/* 
 * File:   C_code.c
 * Author: Chris
 *
 * Created on July 21, 2024, 7:47 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "config.h"

void __interrupt() voltmeter(void);

volatile unsigned int num;

int main()
{
    unsigned int whole, frac, total;
    TRISB = 0x00;
    PORTB = 0x00;
    // 1 - Configure the A/D Module

    // * Configure analog pins, voltage reference and digital I/O 
    // AN0 is connected to wipe of potentiometer
    // Reference voltages are VSS and VDD
    ADCON1 = 0x0E;
    TRISAbits.RA0 = 1;

    // * Select A/D acquisition time
    // * Select A/D conversion clock
    // Right justified, ACQT = 2 TAD, ADCS = FOSC/2
    ADCON2bits.ADCS = 0; // FOSC/2
    ADCON2bits.ACQT = 1; // ACQT = 2 TAD
    ADCON2bits.ADFM = 1; // Right justified

    // * Select A/D input channel
    ADCON0bits.CHS = 0; // Channel 0 (AN0)

    // * Turn on A/D module
    ADCON0bits.ADON = 1;
   
    // 2 - Configure A/D interrupt (if desired)
    // * Clear ADIF bit
    // * Set ADIE bit
    // * Select interrupt priority ADIP bit
    // * Set GIE bit
    // ---> This version is not utilizing interrupt
    PIR1bits.ADIF = 0;
    PIE1bits.ADIE = 1;
    IPR1bits.ADIP = 1;
    RCONbits.IPEN = 0; // disable priority levels
    INTCONbits.PEIE = 1; // enable peripheral interrupts
    INTCONbits.GIE = 1;


    // 3 - Wait the required acquisition time (if required)
    // ---> using ACQT = 2 TAD, no need to manually specify a wait    
    
    while(1)
    {
        ADCON0bits.GO = 1; 
        
        total = (num*50)/1023;
        whole = total /10;
        frac = total % 10;
        
        //Display
        PORTB = (whole << 4) | frac;
    }
    return (EXIT_SUCCESS);
}


void __interrupt() voltmeter(void)
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
        
        num = (ADRESH << 8) | ADRESL;

    }
}