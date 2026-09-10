#include <xc.h>
#include "i2c.h"
#include "ds1307.h"
#include "clcd.h"
extern char time[];
void display_time(unsigned char *clock_reg)
{
    
    
    //HH
    time[0]=((clock_reg[0]>>4) &(0x03)) + '0';
    time[1]=(clock_reg[0] & 0x0F) + '0';
    time[2] = ':';
    //MM
    time[3] =((clock_reg[1]>>4) & 0x07) + '0';
    time[4] = (clock_reg[1] & 0x0F) + '0';
    time[5] = ':';
    
    //SS
    time[6] = ((clock_reg[2]>>4) & 0x07) +'0';
    time[7] = (clock_reg[2] & 0x0F) + '0';
    
    time[8] = '\0';
    
    clcd_print(time,LINE2(0));
       
}

void get_time(unsigned char *clock_reg)
{
    //DATA IS IN BCD format
    clock_reg[2] = read_ds1307(SEC_ADDR);
    clock_reg[1] = read_ds1307(MIN_ADDR);
    clock_reg[0] = read_ds1307(HOUR_ADDR);
}