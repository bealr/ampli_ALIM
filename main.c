/*
 * File:   main.c
 * Author: rbeal
 *
 * Created on October 16, 2016, 12:55 PM
 */

// CONFIG1
#pragma config FOSC = INTOSCIO  // Oscillator Selection bits (INTRC oscillator; port I/O function on both RA6/OSC2/CLKO pin and RA7/OSC1/CLKI pin)
#pragma config WDTE = OFF        // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RA5/MCLR/VPP Pin Function Select bit (RA5/MCLR/VPP pin function is MCLR)
#pragma config BOREN = ON      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EE Memory Code Protection bit (Code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off)
#pragma config CCPMX = RB3      // CCP1 Pin Selection bit (CCP1 function on RB0)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// CONFIG2
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor enabled)
#pragma config IESO = ON        // Internal External Switchover bit (Internal External Switchover mode enabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include "I2C_d.h"

void init(void);

char i2c_trame[5];
char pointer_i2c;
char soft_interrupt;

 static void interrupt isr(void)
{
    PORTAbits.RA6 = 0;
    GIE = 0;
    
    if (PIR1bits.SSPIF) // only write is gerède
    {
        if (SSPSTATbits.S) {
            pointer_i2c = 0;
        }
        
        *(i2c_trame+pointer_i2c) = SSPBUF;
        pointer_i2c++;
            
        if (SSPSTATbits.P) {
            if (*(i2c_trame) == I2C_ADDR)
                soft_interrupt |= 0x01;
        }
        
        PIR1bits.SSPIF = 0;
    }
    
    GIE = 1;
    //PORTAbits.RA6 = 0;
}

void main(void) {
    
    init();
    pointer_i2c = 0;
    soft_interrupt = 0;
    
    PORTBbits.RB0 = 0; // puissance
    PORTBbits.RB2 = 0; // puissance 
    PORTBbits.RB5 = 0; // preamp
    PORTAbits.RA7 = 0; // preamp
    PORTAbits.RA6 = 1;
    
    I2C_init(I2C_ADDR);
    PEIE = 1;
    GIE = 1;
    
    while(1)
    {
        if (soft_interrupt) {
            
            if (soft_interrupt & 0x01) {
                soft_interrupt &= 0xFE;
                
                if (*(i2c_trame + 1) == 0x00) { // puissance
                    if (*(i2c_trame + 2) == 0x00) {
                        PORTBbits.RB0 = 0; // puissance
                        PORTBbits.RB2 = 0; // puissance 
                    }
                    
                    if (*(i2c_trame + 2) == 0x01) {
                        PORTBbits.RB0 = 1; // puissance
                        PORTBbits.RB2 = 1; // puissance 
                    }
                }
                
                if (*(i2c_trame + 1) == 0x01) { // preamp
                    if (*(i2c_trame + 2) == 0x00) {
                        PORTBbits.RB5 = 0; // preamp
                        PORTAbits.RA7 = 0; // preamp
                    }
                    
                    if (*(i2c_trame + 2) == 0x01) {
                        PORTBbits.RB5 = 1; // preamp
                        PORTAbits.RA7 = 1; // preamp
                    }
                }
            }
        }
    }
    
    
    return;
}

void init(void)
{
    // oscillator
    OSCCON = 0b01111110;
    
    TRISBbits.TRISB0 = 0;
    TRISBbits.TRISB2 = 0;
    TRISBbits.TRISB5 = 0;
    TRISAbits.TRISA7 = 0;
    TRISAbits.TRISA6 = 0;
    
}