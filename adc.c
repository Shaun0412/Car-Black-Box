
#include <xc.h>
#include "adc.h"
void adc_init(void)
{
    
    
    
    
    ADCS2 = 0;
    ADCS1 = 1;
    ADCS0 = 0; // FOSC/32 -> adc clock selection
    
    //AD enable
    ADON = 1;
    
    //Right justified
    ADFM = 1;
    
    
    //POrt selection and config bits
    PCFG3 = 0;
    PCFG2 = 0;
    PCFG1 = 0;
    PCFG0 = 0;
    
    
    
    
    
    
}


unsigned int read_adc(unsigned int channel)
{
    //start conversion
    ADCON0 &= ~(7u<<3);
    ADCON0 |= (channel<<3);
     // Acquisition time
    __delay_us(20);
    GO = 1;
    while(GO);
    return (((unsigned int)ADRESH << 8) |ADRESL);
    
}