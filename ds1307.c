#include "ds1307.h"
#include "i2c.h"
#include <xc.h>



void init_ds1307()
{
    //clear the CH(clock  halt) bit for starting oscillator
    unsigned char dummy = read_ds1307(SEC_ADDR);
    dummy = dummy & (0x7F); //bit 7 is CH
    write_ds1307(dummy, SEC_ADDR); // cleared ch bit   
}

unsigned char read_ds1307(unsigned char addr)
{
    i2c_start();
    i2c_write(SLAVE_WRITE); //unique id + write bit
    i2c_write(addr); //passing the address from which we want to read from
    i2c_rep_start();
    i2c_write(SLAVE_READ);
    unsigned char data = i2c_read(NOT_ACKN);
    i2c_stop();
    return data;
       
}


void write_ds1307(unsigned char data, unsigned char addr)
{
    i2c_start();
    i2c_write(SLAVE_WRITE); //passing unique id with intention
    i2c_write(addr); //writing address we want to write to
    i2c_write(data); //writing data to the previously given address
    i2c_stop();
}