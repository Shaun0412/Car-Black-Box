#include "xc.h"
#include "keypad.h"
#include "main.h"

void digital_keypad_init()
{
   //port B switches
    KEYPORT |= 0x3F; //setting RB0 to RB5 as input
    

}

unsigned char read_digital_keypad(unsigned char mode)
{
    static unsigned char key_state = ALL_RELEASED;
    unsigned char key;

    key = PORTB & 0x3F;

    if (mode == STATE)
    {
        /* Detect new press */
        if (key != ALL_RELEASED && key_state == ALL_RELEASED)
        {
            __delay_ms(20);

            key = PORTB & 0x3F;

            if (key != ALL_RELEASED)
            {
                key_state = key;
                return key;
            }
        }

        /* Detect release */
        if (key == ALL_RELEASED)
        {
            __delay_ms(20);

            if ((PORTB & 0x3F) == ALL_RELEASED)
            {
                key_state = ALL_RELEASED;
            }
        }

        return ALL_RELEASED;
    }

    return key;
}