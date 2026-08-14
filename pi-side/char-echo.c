#include "rpi.h"
#include "echo-defs.h"

#define US_DELAY 100000

void notmain()
{
    // assuming uart is already enabled
    u8 c;
    do
    {
        c = uart_get8();
        // wait to let the OS machine catch up
        delay_us(US_DELAY);
        uart_put8(c);
    } while (c != SPECIAL_CHAR);
}
