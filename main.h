/* 
 * File:   main.h
 * Author: shaunvarghese
 *
 * Created on September 9, 2026, 12:32 PM
 */

#ifndef MAIN_H
#define	MAIN_H



#define _XTAL_FREQ 20000000

#include "timer1.h"


void clear_display();
void timer1_delay(int seconds);

void dashboard_screen_setup();
#endif	/* MAIN_H */

