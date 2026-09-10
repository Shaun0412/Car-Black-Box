//#include <xc.h>
#include "keypad.h"
#include "clcd.h"
#include "adc.h"
#include "ds1307.h"
#include "i2c.h"
#include "ext_eeprom.h"
#include "menu.h"
#include "main.h"

#define PASSWORD_ENTERING_DELAY 5

extern volatile unsigned char one_second_flag;

extern unsigned char event_count;

unsigned char time_changed_flag = 0;

//RB4->0,RB5->1

char original_password[5] = "1111";

unsigned char validate_password()
{
    clear_display();
    clcd_print("ENTER PASSWORD",LINE1(0));
    
    
    int wait = 0;
    
    
    char entered_password[5];
    unsigned char key;
    entered_password[4] = '\0';
    for(int i=0;i<4;i++)
    {
        wait = 0;
        do
        {
            key = read_digital_keypad(STATE);
            if(key == ALL_RELEASED)
            {
                if(one_second_flag)
                {
                    one_second_flag = 0;
                    ++wait;
                }
                if(wait >= PASSWORD_ENTERING_DELAY)
                {
                    return INCOMPLETE_PASSWORD;
                }
            }

        }while(key == ALL_RELEASED);
        
        if(key == SW4)
        {
            
            entered_password[i] = '0';
            clcd_putch('*',LINE2(i));
        }
        else if(key == SW5)
        {
            
            entered_password[i] = '1';
            clcd_putch('*',LINE2(i));
        }
        else if(key!=ALL_RELEASED)
        {
            i--; // invalid keypress do the iteration again
            
        }
        
        
    }//after the password is entered
    
    if(original_password[0] == entered_password[0] && original_password[1] == entered_password[1] &&
            original_password[2] == entered_password[2] && original_password[3] == entered_password[3])
            return CORRECT_PASSWORD;
    else
    {
        return WRONG_PASSWORD; //wrong password
    }
    
    
}

char *menu_options[MENU_OPTIONS_COUNT] = {"View Log","Clear Log","Download Log","Set Time","Change Password"};
unsigned char line1_index = 0,line2_index = 1;
unsigned char selected_line = SELECT_LINE1;


void menu_handling()
{
    clear_display();
    unsigned char selected_option_index = 0;
    unsigned char key;
    unsigned char change = 1;
    while(1)
    {
        key = read_digital_keypad(STATE);
         
        if(change)
        {
            clear_display();
            if(selected_line == SELECT_LINE1)
            {
                clcd_putch('*', LINE1(0));
                clcd_putch(' ', LINE2(0));
            }
            else if(selected_line == SELECT_LINE2)
            {
                clcd_putch(' ', LINE1(0));
                clcd_putch('*', LINE2(0));
            }
            clcd_print(menu_options[line1_index], LINE1(1));
            clcd_print(menu_options[line2_index], LINE2(1));
            change = 0;
        }

        if(key == UP_SCROLL_BTN)
        {
            //scroll_up/decrement
            if(selected_line == SELECT_LINE2)
            {
                selected_line = SELECT_LINE1;
                change = 1;
            }
            else if(line1_index!=0 )
            {
                
                --line1_index;
                --line2_index;
                change = 1;
            }
        }
        else if(key == DOWN_SCROLL_BTN)
        {
            //scroll down/increment
            if(selected_line == SELECT_LINE1)
            {
                selected_line = SELECT_LINE2;
                change = 1;
            }
            else if(line2_index != MENU_OPTIONS_COUNT - 1)
            {
                selected_line = SELECT_LINE2;
                ++line1_index;
                ++line2_index;
                change = 1;
            }
        }
        else if(key == MENU_SELECT_BTN)
        {
            if(selected_line == SELECT_LINE1)
            {
                selected_option_index = line1_index;
            }
            else if(selected_line == SELECT_LINE2)
            {
                selected_option_index = line2_index;
                
            }
            menu_selection(selected_option_index);
            if(selected_option_index == SET_TIME)
            {
                
                if(time_changed_flag == 1)
                {
                    dashboard_screen_setup();
                    return;
                }
            }
            change = 1;
            continue;
        }
        else if(key == BACK_BUTTON)
        {
            //go back to dashboard
            dashboard_screen_setup();
            return;
        }
    }
        
    
}

void menu_selection(unsigned char selection_option)
{
    
    switch(selection_option)
    {
        case VIEW_LOG:
        {
            view_log();
            break;
        }
        case CLEAR_LOG:
        {
            clear_log();
            break;
        }
        case DOWNLOAD_LOG:
        {
            break;
        }
        case SET_TIME:
        {
            set_time();
            break;
        }
        case CHANGE_PASSWORD:
        {
            break;
        }
    }
}

void view_log()
{
    clear_display();
    if(event_count == 0)
    {
        clcd_print("No Events Found",LINE1(0));
        timer1_delay(3);
        clear_display();
        return;
    }
    event_t event;
    unsigned char event_index = 0;
    unsigned char key;
    
    unsigned char change = 1;   // Display initially
    while(1)
    {
        if(change)
        {
            clear_display();
            read_log(&event,event_index);
            print_log_event(&event,1);
            if(event_count>1)
            {
                read_log(&event,event_index+1);
                print_log_event(&event,2);
            }
            else
            {
                clcd_print("                ", LINE2(0));
            }
            change = 0;
        }
        key = read_digital_keypad(STATE);   
        if(key == SW3)
        {
            //scroll up decrement index
            if(event_index > 0)
            {
                event_index--;
                change = 1;
            }
            
        }
        else if(key == SW4)
        {
            //scroll down decremtn index
            if(event_index + 1 < event_count -1 )
            {
                ++event_index;
                change = 1;
            }   
        }
        else if(key == SW0)
        {
            clear_display();
            return;
        }
    }
    return;

}
void print_log_event(event_t *event,int line)
{
    if(line == 1)
    {
        //print the event log at line 1
        clcd_print(event->hour,LINE1(0));
        clcd_putch(':',LINE1(2));
        clcd_print(event->minute,LINE1(3));
        clcd_putch(':',LINE1(5));
        clcd_print(event->second,LINE1(6));
        clcd_putch(' ',LINE1(8));
        clcd_print(event->gear,LINE1(9));
        clcd_putch(':',LINE1(11));
        clcd_print(event->speed,LINE1(12));
        

    }
    else if(line == 2)
    {
        //print the event log at line 1
        clcd_print(event->hour,LINE2(0));
        clcd_putch(':',LINE2(2));
        clcd_print(event->minute,LINE2(3));
        clcd_putch(':',LINE2(5));
        clcd_print(event->second,LINE2(6));
        clcd_putch(' ',LINE2(8));
        clcd_print(event->gear,LINE2(9));
        clcd_putch(':',LINE2(11));
        clcd_print(event->speed,LINE2(12));
        

    }
}
void read_log(event_t *events,unsigned char event_index)
{
    unsigned int address_counter = 0;
    address_counter = address_counter + event_index*10;
    events->hour[0] = read_external_eeprom(address_counter++); 
    events->hour[1] = read_external_eeprom(address_counter++); 
    events->hour[2] = '\0';
    events->minute[0] = read_external_eeprom(address_counter++); 
    events->minute[1] = read_external_eeprom(address_counter++); 
    events->minute[2] = '\0'; 
    events->second[0] = read_external_eeprom(address_counter++); 
    events->second[1] = read_external_eeprom(address_counter++); 
    events->second[2] = '\0'; 
    events->gear[0] = read_external_eeprom(address_counter++); 
    events->gear[1] = read_external_eeprom(address_counter++); 
    events->gear[2] = '\0'; 
    events->speed[0] = read_external_eeprom(address_counter++); 
    events->speed[1] = read_external_eeprom(address_counter++); 
    events->speed[2] = '\0'; 
    return;

}


void clear_log()
{
    clear_display();
    int second = 0;
    event_count = 0;
    clcd_print("Logs Cleared...",LINE1(0));
    while(1)
    {
        if(one_second_flag)
        {
            ++second;
            one_second_flag = 0;
            if(second == 2)
            {
                clear_display();
                break;
            }
        }
    }
    
}


void set_time()
{
    unsigned char previous_time[3];
    unsigned char key;
    unsigned char field = SECOND_FIELD;
    get_time(previous_time);
    clear_display();
    clcd_print("Set time",LINE1(0));

    unsigned char hour,minute,second;

    hour = ((previous_time[0]>>4) *10) + (previous_time[0] & 0x0F);
    minute = ((previous_time[1]>>4) *10) + (previous_time[1] & 0x0F);
    second = ((previous_time[2]>>4) *10) + (previous_time[2] & 0x0F);
    unsigned char change = 1;

    unsigned char time[9];
    unsigned int delay = BLINK_DELAY;
    unsigned char blink = 0;
    while(1)
    {
        
        key = read_digital_keypad(STATE);
        

        if(key == SW3)
        {
            //decrement
            if(field == SECOND_FIELD)
            {
                if(second == 0)
                {
                    second = 59;
                }
                else
                    --second;
            }
            else if(field == MINUTE_FIELD)
            {
                if(minute == 0)
                {
                    minute = 59;
                }
                else
                    --minute;
            }
            else if(field == HOUR_FIELD)
            {
                if(hour == 0)
                {
                    hour = 23;
                }
                else
                    --hour;

            }
            change = 1;
        }
        else if(key == SW4)
        {
            //increment
            if(field == SECOND_FIELD)
            {
                if(second == 59)
                {
                    second = 0;
                }
                else
                    ++second;
            }
            else if(field == MINUTE_FIELD)
            {
                if(minute == 59)
                {
                    minute = 0;
                }
                else
                    ++minute;
            }
            else if(field == HOUR_FIELD)
            {
                if(hour == 23)
                {
                    hour = 0;
                }
                else
                    ++hour;
            }
            change =1;
        }
        else if(key == SW2)
        {
            //change field
            if(field >= HOUR_FIELD)
            {
                field = SECOND_FIELD;
            }
            else
                ++field;
            change = 1;
        }
        else if(key == SW0)
        {
            return;
        }
        else if(key == SW5)
        {
            clear_display();
            clcd_print("Press RB5-Confirm",LINE1(0));
            clcd_print("      RB0-Go back",LINE2(0));
            do
            {
                key = read_digital_keypad(STATE);
            }while(key != SW5 && key != SW0);
            if(key == SW5)
            {
                //set the value in time to RTC
                unsigned bcd_sec,bcd_min,bcd_hour;
                bcd_sec = ((second/10)<<4) | (second%10);
                write_ds1307(bcd_sec,SEC_ADDR);
                bcd_min = ((minute/10)<<4) | (minute%10);
                write_ds1307(bcd_min,MIN_ADDR);
                bcd_hour = ((hour/10)<<4) | (hour%10);
                write_ds1307(bcd_hour,HOUR_ADDR);
                time_changed_flag = 1;
                return;
            }
            else 
            {
                time_changed_flag =0;
                return;
            }
        }
        //----- if user changes time
        if(change )
        {
            blink = 0; //resetting blink, 0= lightup, 1= turn off
            time[0] = (hour / 10) + '0';
            time[1] = (hour % 10) + '0';
            time[2] = ':';

            time[3] = (minute / 10) + '0';
            time[4] = (minute % 10) + '0';
            time[5] = ':';

            time[6] = (second / 10) + '0';
            time[7] = (second % 10) + '0';

            time[8] = '\0';
            change = 0;
            clcd_print(time,LINE2(0));
        }
        //-----TIMER
        if(one_second_flag)
        {
            one_second_flag = 0;
            blink = !blink;
        }
        //-----BLINKING algo
        if(blink)
        {
            if(field == SECOND_FIELD)
            {
                clcd_putch(' ',LINE2(6));
                clcd_putch(' ',LINE2(7));
            }
            else if(field == MINUTE_FIELD)
            {
                clcd_putch(' ',LINE2(3));
                clcd_putch(' ',LINE2(4));
            }
            else if(field == HOUR_FIELD)
            {
                clcd_putch(' ',LINE2(0));
                clcd_putch(' ',LINE2(1));
            }
        }
        if(!blink)
        {
            if(field == SECOND_FIELD)
            {
                clcd_putch(time[6],LINE2(6));
                clcd_putch(time[7],LINE2(7));
            }
            else if(field == MINUTE_FIELD)
            {
                clcd_putch(time[3],LINE2(3));
                clcd_putch(time[4],LINE2(4));
            }
            else if(field == HOUR_FIELD)
            {
                clcd_putch(time[0],LINE2(0));
                clcd_putch(time[1],LINE2(1));
            }

        }
        
    }
}

void change_password()
{
    unsigned char key;
    for(int i=0;i<4;i++)
    {
        
        do
        {
            key = read_digital_keypad(STATE);
            if(key == SW4)
                original_password[i] = '0';
            else if(key == SW5)
                original_password[i] = '1';
            else if(key != ALL_RELEASED)
                --i;
        }while(key == ALL_RELEASED);
    }
    
}