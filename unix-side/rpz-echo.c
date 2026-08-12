// simple program where rpi echos input
// a little fancier than that found in test-uart/, cuz
// of surrounding infrastructure

#include <stdio.h>
#include "libunix.h"

int main(void)
{
    printf("will call find_last; ");
    char *last = find_ttyusb_last();
    printf("last=<%s>\n", last);
    printf("will call find_first; ");
    char *first = find_ttyusb_first();
    printf("first=<%s>\n", first);

    return 0;
}
