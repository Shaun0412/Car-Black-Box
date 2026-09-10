#include "ext_eeprom.h"
#include "i2c.h"
#include <xc.h>




unsigned char read_external_eeprom(unsigned int addr)
{
    i2c_start();
    i2c_write(EXT_EEPROM_SLAVE_WRITE); //unique id + write bit
    i2c_write(addr); //passing the address from which we want to read from
    i2c_rep_start();
    i2c_write(EXT_EEPROM_SLAVE_READ);
    unsigned char data = i2c_read(NOT_ACKN);
    i2c_stop();
    
    
    return data;
       
}


void write_external_eeprom(unsigned char data, unsigned int addr)
{
    i2c_start();
    i2c_write(EXT_EEPROM_SLAVE_WRITE); //passing unique id with intention
    i2c_write(addr); //writing address we want to write to
    i2c_write(data); //writing data to the previously given address
    i2c_stop();
    
    
    for(int wait=3000;wait--;); //this is mandatory delay for external eeprom write
}
