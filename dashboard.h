/* 
 * File:   dashboard.h
 * Author: shaunvarghese
 *
 * Created on September 10, 2026, 3:04 PM
 */

#ifndef DASHBOARD_H
#define	DASHBOARD_H

void dashboard(unsigned char key);
void dashboard_screen_setup();

void store_event(unsigned int address);
void store_eeprom();

#endif	/* DASHBOARD_H */

