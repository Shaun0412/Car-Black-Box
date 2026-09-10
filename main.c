/* 
 * File:   main.c
 * Author: shaunvarghese
 *
 * Created on September 5, 2026, 1:05 PM
 */

#pragma config WDTE = OFF


#include "xc.h"
#include "keypad.h"
#include "clcd.h"
#include "adc.h"
#include "ds1307.h"
#include "i2c.h"
#include "ext_eeprom.h"
#include "dashboard.h"

#include "main.h"
#include "menu.h"
volatile unsigned char timer1_count = 0;
volatile unsigned char one_second_flag = 0;

unsigned char event_count = 0;;


char time[9];
unsigned char spd[3]="00";
char *gears[8] = {"GN","G1","G2","G3","G4","G5","GR","_C"};
unsigned char current_gear=0;

int change = 1;

char display_clear[17]="                ";

unsigned char attempt = MAX_PASSWORD_ATTEMPT;

void init_config(void) 
{
    i2c_init(100000);//100kHz 
    init_ds1307();
    digital_keypad_init();
    init_clcd();
    adc_init();
    init_timer1();
    clcd_print("TIME      EV  SP",LINE1(0));
    

}


void main(void) 
{
    init_config();
    int second = 0;
    unsigned char key,ret = NULL_PASSWORD;
    while (1) 
    {
        key = read_digital_keypad(STATE);
        
        if(key == SW3 || ret == WRONG_PASSWORD)
        {
            
            ret = validate_password();
            if(ret == INCOMPLETE_PASSWORD)
            {
                dashboard_screen_setup();
                dashboard(ALL_RELEASED);
            }
            else if(ret == CORRECT_PASSWORD)
            {
                attempt = MAX_PASSWORD_ATTEMPT;
                unsigned int second = 0;
                clear_display();
                clcd_print("Logging in..",LINE1(0));
                while(1)
                {
                    if(one_second_flag)
                    {
                        ++second;
                        if(second == 2)
                        {
                            clear_display();
                            break;
                        }
                    }
                }
                menu_handling();
            }
            else if(ret == WRONG_PASSWORD)
            {
                attempt--;
                if(attempt)
                {
                    
                    clear_display();
                    clcd_print("WRONG PASSWORD",LINE1(0));
                    clcd_putch(attempt + '0',LINE2(0));
                    clcd_print("attempts left",LINE2(2));
                    __delay_ms(1000);
                }
                else
                {
                    attempt = MAX_PASSWORD_ATTEMPT;
                    second = 30;
                    clear_display();
                    clcd_print("System Blocked",LINE1(0));
                    clcd_print("Wait for ...",LINE2(0));
                    clcd_putch('s',LINE2(15));
                    while(second)
                    {
                        if(one_second_flag)
                        {
                            --second;
                            one_second_flag = 0;
                        }
                        clcd_putch((second / 10) + '0', LINE2(13));
                        clcd_putch((second % 10) + '0', LINE2(14));
                    }
                    if(second == 0)
                    {
                        clcd_print(display_clear,LINE2(0));
                        ret = NULL_PASSWORD;
                    }
                }
                
                
            }
            
        }
        else
        {
            clcd_print("TIME      EV  SP",LINE1(0));
            dashboard(key);
        }
        
    }

}

void clear_display()
{
    clcd_print(display_clear,LINE1(0));
    clcd_print(display_clear,LINE2(0));
}


int timer1_delay(int seconds)
{
    while(1)
    {
        if(one_second_flag)
        {
            --seconds;
            if(seconds == 0)
            {
                                
                break;
            }
        }
    }
}