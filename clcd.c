

#include "clcd.h"
#include "xc.h"

void init_clcd()
{
    
    TRISD = 0x00; //PORTD is output from MC
    TRISE1 = 0; //RS and EN lines in PORTE as out
    TRISE2 = 0;//rs
    clcd_display_controller_init();
    
    
    
    
}


void clcd_display_controller_init()
{
    __delay_ms(30); // initial time for CLCD to setup, specified in DS
    
    
    //we do it three times because we don't know in what mode clcd was and how the data will be interpreted
    clcd_write(0x33,INST_MODE);
    __delay_us(4100);
    clcd_write(0x33,INST_MODE);
    __delay_us(100);
    clcd_write(0x33,INST_MODE);
    __delay_us(1);
    
    //8 bit mode, 16x2 font
    clcd_write(TWOLINES_8BIT_CLCD_MODE,INST_MODE);
    __delay_us(100);
    
    //clearing display
    clcd_write(CLEAR_DISP_SCREEN,INST_MODE);
    __delay_us(100);
    
    clcd_write(DISP_ON_AND_CURSOR_OFF,INST_MODE);
    __delay_us(100);
}


void clcd_print(const char str[],unsigned char addr)
{
    clcd_write(addr,INST_MODE);
    while(*str != '\0')
    {
        clcd_write(*str,DATA_MODE);
        ++str;
    }   
    
}


void clcd_putch(const char data, unsigned char addr)
{
    clcd_write(addr,INST_MODE);
    clcd_write(data,DATA_MODE);   
}   

void clcd_write(unsigned char byte, unsigned char mode)
{
    
    CLCD_RS = mode;
    CLCD_DATA_PORT = byte;
    CLCD_EN = HI;
    __delay_us(100);
    CLCD_EN = LOW;
    
    
    __delay_us(4100);//recommended waiting time
}
    