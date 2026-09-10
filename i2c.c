
#include "xc.h"
#include "i2c.h"
void i2c_init(unsigned long baud)
{
    
    //slew rrate control bit, standard speed operation
    SMP = 1;
    
    
    //configuring MSSP as master device
    SSPM3 = 1; SSPM2 = SSPM1 = SSPM0 = 0;
    
    //configs synchronous serial ports, enables SSP
    SSPEN = 1;
    
    //SCL frequecny setting
    SSPADD = (unsigned char) (FOSC/(4*baud)) -1; //for standard 100khz op, we can just pass 49 to SSPADD
}

void i2c_wait_for_idle(void)
{
    while(R_nW ||(SSPCON2 & 0x1F)); //R_nW is R/W bit, refer page 85 
        
}

void i2c_start(void)
{
    
    i2c_wait_for_idle(); //waits till BUS is idle and then start
    
    //start condition enable bit
    SEN = 1;
}


void i2c_rep_start(void)
{
    i2c_wait_for_idle(); //waits till BUS is idle and then start
    
    //Repeated start bit
    RSEN = 1;
    
}

void i2c_stop(void)
{
    i2c_wait_for_idle(); //waits till BUS is idle and then stop
    
    //stop condition enable bit
    PEN = 1;
}


unsigned int i2c_write(unsigned char data)
{
    i2c_wait_for_idle();
    SSPBUF = data;
    
    
    return !ACKSTAT; //it returns status of acknowledgement 0 means acknowledgment received from slave
    
}

unsigned char i2c_read(unsigned char ack)
{
    unsigned char data;
    i2c_wait_for_idle();
    
    RCEN = 1; // Receive enable bit
    
    i2c_wait_for_idle();
    data = SSPBUF;
    
    if(ack == NOT_ACKN)
    {
        ACKDT = 1; // receiving stops after receiving 1 byte of data
    }
    
    else
    {
        ACKDT = 0; //continuos receiving because we acknowledged
    }
        
    ACKEN = 1;// enabling transmit of acknowledment bit
    
    return data;
  
}