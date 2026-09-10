/* 
 * File:   keypad.h
 * Author: shaunvarghese
 *
 * Created on July 31, 2026, 10:22 PM
 */

#ifndef KEYPAD_H
#define	KEYPAD_H


#define KEYPORT TRISB


#define LEVEL 5
#define STATE 6

#define ALL_RELEASED 0x3F
#define SW0 0x3E
#define SW1 0x3D
#define SW2 0x3B
#define SW3 0x37
#define SW4 0x2F
#define SW5 0x1F

void digital_keypad_init(void);

unsigned char read_digital_keypad(unsigned char mode);


#endif	/* KEYPAD_H */

