// inspired by Dawson Engler's bootloader for CS140E loads a program to a rpi
// zero connected via a tty-USB device

#include <stdio.h>

#include "libunix.h"

int main(void)
{
    char* my_str = "affix";
    printf("str is %s\n", my_str);
    printf("prefix of str is af? %s\n", 
        prefix_cmp(my_str, "af") ? "true" : "false");
    printf("suffix of str is fix? %s\n", 
        suffix_cmp(my_str, "fix") ? "true" : "false");

    return 0;
}
