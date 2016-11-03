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

char command_i2c, tmp;

 static void interrupt isr(void)
{
    PORTAbits.RA6 = 0;
    GIE = 0;
    
    if (PIR1bits.SSPIF)
    {
        tmp = SSPBUF;
        if (SSPSTATbits.D_nA)
            command_i2c = tmp;
            
        PIR1bits.SSPIF = 0;
    }
    
    GIE = 1;
    //PORTAbits.RA6 = 0;
}

void main(void) {
    
    init();
    
    
    
    PORTBbits.RB0 = 0;
    PORTBbits.RB2 = 0;
    PORTBbits.RB5 = 0;
    PORTAbits.RA7 = 0;
    PORTAbits.RA6 = 1;
    
    //__delay_ms(5000);
    
    I2C_init(I2C_ADDR);
    PEIE = 1;
    GIE = 1;
    
    while(1)
    {
        if (command_i2c)
        {
            if (command_i2c & 0x01)
                PORTBbits.RB0 = 1;
            else
                PORTBbits.RB0 = 0;
            
            if (command_i2c & 0x02)
                PORTBbits.RB2 = 1;
            else
                PORTBbits.RB2 = 0;
            
            command_i2c = 0;
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