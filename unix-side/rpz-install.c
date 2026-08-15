// inspired by Dawson Engler's bootloader for CS140E loads a program to a rpi
// zero connected via a tty-USB device

#include <stdio.h>
#include <getopt.h>

#include "libunix.h"

static int f_verbose;

static struct option long_options[] =
{
    {"baud-rate", required_argument, NULL,     'b'},
    {"last",      no_argument,       NULL,     'l'},
    {"first",     no_argument,       NULL,     'f'},
    {"device",    required_argument, NULL,     'd'},
    {"verbose",   no_argument,       &f_verbose, 1},
    // sentinel
    {NULL,        0,                 NULL,      0 }
};

// MUST TODO: add a usage function that prints how to use the bootloader


int main(int argc, char ** argv)
{
    char * this_prog = argv[0];
    uint32_t baud = B115200;
    char * dev_name = "";
    char * pi_prog = "";

    int opt_index;
    int c;
    while (1)
    {
        // the leading '-' tells getopt to not reorder argv
        // the ':' after the leading '-' tells getopt that we'll handle errors
        c = getopt_long(argc, argv, "-:b:lfd:v", long_options, &opt_index); 
        if (c == -1)
            break;
        switch (c)
        {
            // TODO: handle more than one non-opt arg; can only bootload one
            // prog!!
            case 1:
                printf("got prog <%s>\n", optarg);
                pi_prog = optarg;
                break;
            // TODO: parse baud rate; only allow those defined in termios.h
            case 'b':
                printf("changing baud rate to <%s>\n", optarg);
                break;
            case 'l':
                printf("will look for last device\n");
                break;
            case 'f':
                printf("will look for first device\n");
                break;
            // bogus device handled by find_tty
            case 'd':
                printf("will look for device <%s>\n", optarg);
                break;
            // consider editing array entry because decoupled; seems simple
            // enough to not cause problems, however
            case 'v':
                f_verbose = 1;
            case '?':
                fprintf(stderr, "Unknown option <%c>\n", optopt);
                break;
            case ':':
                fprintf(stderr, "Missing argument for option <%c>\n", optopt);
                break;
            default:
                fprintf(stderr, "??? getopt returned character code <%c>\n",
                    c);
                break;
        }
    }
    if (f_verbose)
    {
        printf("verbose mode activated\n");
        printf("bootloader settings:\n");
        printf("\t baud-rate: %u\n", baud);
        printf("\t    device: %s\n", dev_name);
        printf("\tpi-program: %s\n", pi_prog);
    }

    return 0;
}
