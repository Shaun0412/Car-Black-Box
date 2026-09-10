/* 
 * File:   adc.h
 * Author: shaunvarghese
 *
 * Created on August 28, 2026, 11:42 AM
 */

#ifndef ADC_H
#define	ADC_H

#define _XTAL_FREQ 20000000

void adc_init(void);
unsigned int read_adc(unsigned int);


#endif	/* ADC_H */

