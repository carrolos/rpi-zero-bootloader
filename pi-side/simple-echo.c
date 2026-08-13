#include "rpi.h"
#include "echo-defs.h"

#define US_DELAY 500000

void notmain()
{
//    for (u8 i = 0; i < 10; i++)
//    {
//        putk("in notmain(), will try to get8 soon!\r\n");
//    }
//    u8 buf[128];
//    u8 *hare = buf;
    // assuming uart is already enabled
    u8 c;
    for (u32 i = 0; i < MAX_RETRIES; i++)
    {
        c = uart_get8();
        // wait to let the OS machine catch up
        delay_us(US_DELAY);
        // ceaser cipher
        uart_put8(c + 3);
//        u8 c;
//        while ((c = uart_get8()) != SPECIAL_CHAR)
//            *hare++ = c;
//        
//        putk("\r\necho: ");
//        for (u8* turtle = buf; turtle != hare; turtle++)
//        {
//            uart_put8(*turtle);
//        }
//        // reset
//        hare = buf;
//        // actually put special char so laptop knows to start txing
//        uart_put8(SPECIAL_CHAR);
    }
}
