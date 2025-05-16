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
#define STOP_LED PORTBbits.RB1

void StopDisplay();
void Display(int s, int d);
void __interrupt(high_priority) emergency_stop(void);
void __interrupt(low_priority) adc(void);

volatile int speed;
volatile int delay;
volatile int speed_10x;
volatile int duty_cycle = 0;
volatile int mode = 0;
volatile int counter = 0;

int main() 
{
    // Configure LCD Pins
    // Data pins connected to PORTD
    TRISD = 0x00; 
    
    // RS = RE0
    // RW = RE1
    // E  = RE2
    TRISE = 0x00;
    
    //INTCON = 0x00;
    // connect the LCD pins to the appropriate PORT pins
    pin_setup(&PORTD, &PORTE);
    
    // initialize the LCD to be 16x2 
    begin(16, 2, LCD_5x8DOTS);
    
    //--------------------------------------------------------------------------
    // Configure PWM
    // PWM Frequency = 1 KHz
    // PWM Period = 1x10^-3 Second
    PR2 =  249;

    // setup timer 2
    // FOSC = 1 MHz
    T2CONbits.T2CKPS = 0b00; // Prescaler 1:1
    T2CONbits.TMR2ON = 1;
    

    // setup CCP2 in PWM mode
    TRISCbits.RC1 = 0;
    CCP2CONbits.CCP2M = 0b1100;

    //--------------------------------------------------------------------------
    
    //Setup all inerrupts
    TRISB = 0x01;
    INTCONbits.GIE = 1; // Globabl interrupt enable
    INTCONbits.PEIE = 1; // enable peripheral interrupts
    INTCONbits.INT0E = 1; // Enable INT0
    INTCONbits.INT0IF = 0; // reset INT0 flag
    INTCON2bits.INTEDG0 = 0; // falling edge
    //Setup interrupt priorities
    RCONbits.IPEN = 1; // enable priority
    INTCONbits.GIEH = 1; // enable all high priority
    INTCONbits.GIEL = 1; // enable all low priority
    
    
    //--------------------------------------------------------------------------
    // setup timer0 
    T0CONbits.PSA = 0; // Prescaler is assigned
    T0CONbits.T0PS = 0x05; // 1:64 prescale value
    T0CONbits.T0CS = 0; // clock source is internal instruction cycle
    T0CONbits.T08BIT = 0; // operate in 16 bit mode now
    T0CONbits.TMR0ON = 1; // Turn on timer
    TMR0 = 65498; // For 10ms second delay (with 1:64 prescaler)
    
    // setting up timer0 as an interrupt
    INTCONbits.TMR0IE = 1;
    INTCONbits.TMR0IF = 0;
    INTCON2bits.TMR0IP = 0; // low priority 
    
    //--------------------------------------------------------------------------
    // Setup the ADC
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
    ADCON2bits.ADFM = 0; // Left justified

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
    IPR1bits.ADIP = 0;

    
    // 3 - Wait the required acquisition time (if required)
    // ---> using ACQT = 2 TAD, no need to manually specify a wait    
    STOP_LED = 1;
    
    while(1)
    {
        ADCON0bits.GO = 1;   
        
        if((mode%2) == 0)
        { 
            Display(speed, delay);
        }
        else
        {
            StopDisplay();
        }
    }

    return (EXIT_SUCCESS);
}

void Display(int s, int d)
{
    home();
    print("Max Speed:");
    print_int(speed);
    print("            ");
    setCursor(0,1);
    print("Delay:");
    print_int(delay);
    print(" ms");
    print("             ");
}

void StopDisplay()
{
    home();
    print("    STOPPED!    ");
    setCursor(0, 1);
    print("                ");
}

void __interrupt(high_priority) stop(void)
{
    if (INTCONbits.INT0IE && INTCONbits.INT0IF)
    {
        INTCONbits.INT0IF = 0;
        STOP_LED = ~STOP_LED; // Turns LED on and off
        mode++; //Changes which state is being displayed
        // Turn off the interrupt flag to avoid recursive interrupt calls
        
        if((mode%2) == 1) //Prevents Timer0 from interrupting
        {
            T0CONbits.TMR0ON = 0; // Turn off timer
        }
        else
        {
            T0CONbits.TMR0ON = 1; // Turn on timer
        }
    }
}

void __interrupt(low_priority) adc(void)
{        
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
            speed = ((ADRESH << 2)  + ((ADRESL >> 6) & 0x03)) & 0x3FF;
            speed_10x = speed;
            ADCON0bits.CHS = 1;
        }   
        else if (ADCON0bits.CHS == 1) // channel AN1 (potentiometer 2)
        {
            delay = (((ADRESH << 2) + ((ADRESL >> 6) & 0x03)) & 0x3FF) * 10;
            ADCON0bits.CHS = 0;
        }
        if(speed_10x < (delay / 10))
        {
            speed_10x *= 10;
        }
        if(speed_10x < (delay / 10))
        {
            speed_10x *= 10;
        }
        if(speed_10x < (delay / 10))
        {
            speed_10x *= 10;
        }
        if(speed_10x < (delay / 10))
        {
            speed_10x *= 10;
        }
    }
    else if(INTCONbits.TMR0IE && INTCONbits.TMR0IF)
    {
        duty_cycle = (speed_10x) / (delay / 10);
        INTCONbits.TMR0IF = 0;
        TMR0 = 65498; // For 10ms delay (with 1:64 prescaler)
    }
    if(speed == 0)
    {
        duty_cycle = 0;
        counter == 0;
    }
    else if(counter > (delay/10))
    {
        duty_cycle = 0;
        counter = 0;
    }
    if(delay == 0)
    {
        duty_cycle = speed;
        counter = 1;
    }
    if((mode%2) == 0)
    {
        CCPR2L = (((duty_cycle * counter) / (speed_10x / speed)) & 0x3FC) >> 2;
        CCP2CONbits.DC2B = (((duty_cycle * counter) / (speed_10x / speed)) & 0x03);
        counter += 1;
    }
    else
    {
        CCPR2L = 0;
        CCP2CONbits.DC2B = 0;
    }
    return;
}







