/* 
 * File:   ext_eeprom.h
 * Author: shaunvarghese
 *
 * Created on September 8, 2026, 3:41 PM
 */

#ifndef EXT_EEPROM_H
#define	EXT_EEPROM_H

#define EXT_EEPROM_SLAVE_WRITE 0XA0
#define EXT_EEPROM_SLAVE_READ 0xA1
#define _XTAL_FREQ 20000000

void write_external_eeprom(unsigned char data, unsigned int addr);
unsigned char read_external_eeprom(unsigned int addr);

void store_eeprom();
#endif	/* EXT_EEPROM_H */

