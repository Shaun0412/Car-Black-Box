/* 
 * File:   menu.h
 * Author: shaunvarghese
 *
 * Created on September 9, 2026, 9:11 PM
 */

#ifndef MENU_H
#define	MENU_H

#define CORRECT_PASSWORD 1
#define WRONG_PASSWORD 0
#define INCOMPLETE_PASSWORD 2
#define NULL_PASSWORD 3

#define MAX_PASSWORD_ATTEMPT 3



#define MENU_OPTIONS_COUNT 5
//SW3,SW4,SW5,SW0
#define UP_SCROLL_BTN 0x37
#define DOWN_SCROLL_BTN 0x2F
#define MENU_SELECT_BTN 0x1F
#define BACK_BUTTON 0x3E



#define SELECT_LINE1  0
#define SELECT_LINE2  1


#define VIEW_LOG 0
#define CLEAR_LOG 1
#define DOWNLOAD_LOG 2
#define SET_TIME 3
#define CHANGE_PASSWORD 4

typedef struct 
{
    unsigned char hour[3];
    unsigned char minute[3];
    unsigned char second[3];
    unsigned char gear[3];
    unsigned char speed[3];
}event_t;
void menu_handling();
void menu_selection(unsigned char selection_option);
void view_log();
void read_log(event_t *events,unsigned char event_index);
void print_log_event(event_t *event,int line);
unsigned char validate_password();
void clear_log();
#endif	/* MENU_H */

