/* 
 * File:   i2c.h
 * Author: shaunvarghese
 *
 * Created on September 1, 2026, 12:01 PM
 */

#ifndef I2C_H
#define	I2C_H
#define FOSC 20000000

#define ACKN 0
#define NOT_ACKN 1

void i2c_init(unsigned long baud);
void i2c_wait_for_idle(void);
void i2c_start(void);
void i2c_rep_start(void);
void i2c_stop(void);
unsigned int i2c_write(unsigned char data);
unsigned char i2c_read(unsigned char ack);

#endif	/* I2C_H */

