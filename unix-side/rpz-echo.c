// simple program where rpi echos input
// a little fancier than that found in test-uart/, cuz
// of surrounding infrastructure

#include <stdio.h>

#include "libunix.h"

int main(void)
{
    char* my_str = "affix";
    output("str is %s\n", my_str);
    output("prefix of str is af? %s\n", 
        prefix_cmp(my_str, "af") ? "true" : "false");
    output("suffix of str is fix? %s\n", 
        suffix_cmp(my_str, "fix") ? "true" : "false");

    return 0;
}
