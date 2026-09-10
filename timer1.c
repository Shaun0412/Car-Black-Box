#include "timer1.h"

#include <xc.h>


extern volatile unsigned char timer1_count;
extern volatile unsigned char one_second_flag;
void init_timer1()
{
    PEIE = 1;
    GIE = 1;
    TMR1IE = 1; //tmr interrupt enable
    
    
    T1CKPS1 = 1;
    T1CKPS0 = 1;  //prescaling 1:8
    
    TMR1ON = 1;//timer enable
    
}


void __interrupt() isr(void)
{
    if(TMR1IF)
    {
        timer1_count++;
        TMR1H = 0x1C;
        TMR1L = 0xDC; //0BDC

        TMR1IF = 0;
        if(timer1_count >= 10)
        {
            timer1_count = 0;
            one_second_flag = 1;
        }
        
    }
}