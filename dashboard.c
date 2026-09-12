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

extern unsigned char current_gear;
extern int change;

extern char time[];
extern unsigned char spd[];
extern char *gears[];
extern unsigned char event_count;
void store_event(unsigned int address)
{
    write_external_eeprom(time[0], address++);
    write_external_eeprom(time[1], address++);

    write_external_eeprom(time[3], address++);
    write_external_eeprom(time[4], address++);

    write_external_eeprom(time[6], address++);
    write_external_eeprom(time[7], address++);

    write_external_eeprom(gears[current_gear][0], address++);
    write_external_eeprom(gears[current_gear][1], address++);

    write_external_eeprom(spd[0], address++);
    write_external_eeprom(spd[1], address++);
}

void store_eeprom()
{
    unsigned int source_address;
    unsigned int destination_address;
    unsigned char i;
    unsigned char j;
    unsigned char data;

    if(event_count < 10)
    {
        store_event(event_count * 10);
        event_count++;
    }
    else
    {
        // Shift events towards the beginning
        for(i = 0; i < 9; i++)
        {
            source_address = (i + 1) * 10;
            destination_address = i * 10;

            for(j = 0; j < 10; j++)
            {
                data = read_external_eeprom(source_address + j);
                write_external_eeprom(data,destination_address + j);
            }
        }

        // Store new event in slot 9
        store_event(90);
    }
}
void dashboard(unsigned char key)
{
    unsigned char display_buf[17]; 
    unsigned char clock_reg[3];
    
    
    //TIME
    get_time(clock_reg);
    display_time(clock_reg);
    
    //GEAR
    if(key == SW0)
    {    
        //collision
        current_gear = 7;
        change = 1;
    }
    else if(key == SW1)
    {
        //increment
        if(current_gear == 7)
            current_gear = 0;
        else
            current_gear = (current_gear+1)%7;
        
        change = 1;
    }
    else if(key == SW2)
    {
        //decrement
        if(current_gear == 7)
        {
            current_gear = 0;
            change = 1;
        }
        else if(current_gear > 0)
        {
            current_gear = (current_gear-1);
            change = 1;
        }
        
     
        
    }
    
    clcd_print(gears[current_gear],LINE2(10));
    
    //SPEED
    unsigned int speed = read_adc(0);

    speed = speed / 10.23;
    
    spd[0] = (speed / 10) + '0';
    spd[1] = (speed % 10) + '0';
    spd[2] = '\0';

    clcd_print(spd, LINE2(13));
    
    if(change)
    {
        
        store_eeprom();
        change = 0;
    }
    
}


void dashboard_screen_setup()
{
    clear_display();
    clcd_print("TIME      EV  SP",LINE1(0));
}
