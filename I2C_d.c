/* 
 * File:   I2C_d.c
 * Author: rbeal
 *
 * Created on October 19, 2016, 6:55 PM
 */

#include "I2C_d.h"
#include <xc.h>


void I2C_init(char addr)
{
    TRISBbits.TRISB1 = 1; // SDA
    TRISBbits.TRISB4 = 1; // SCL
    
    SSPCONbits.CKP = 1;
    
    SSPCONbits.SSPM = 0b0110; // I2C slave, 7b address
    SSPADD = addr<<1;
    
    SSPCONbits.SSPEN = 1; // SSP enable
    PIE1bits.SSPIE = 1; // enable SSP interrupt
    PIR1bits.SSPIF = 0;
}
