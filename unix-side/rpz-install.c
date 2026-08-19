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

static char *this_prog;

// MUST TODO: add a usage function that prints how to use the bootloader
static void usage()
{
    output("\nusage: %s [(-b | --baud_rate) <baud>] [(-l | --last) | (-f | "
        "--first) | (-d | --device) </path/to/device>] [-v | --verbose] "
        "<pi_program>\n",
        this_prog);
}


// returns true if baud is valid; false otherwise
// a valid baud rate is one that is defined in termios.h
static bool check_baud(uint32_t b)
{
    unimplemented();
    return false;
}

int main(int argc, char ** argv)
{
    this_prog = argv[0];
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
            case 1:
                if (pi_prog[0] == '\0')
                {
                    pi_prog = optarg;
                }
                else
                {
                    printf("too many non-option pi programs; bootloader takes"
                        " only one pi program\n");
                    usage();
                }
                break;
            // TODO: parse baud rate; only allow those defined in termios.h
            case 'b':
                check_baud(atoi(optarg));
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
                break;
            case '?':
                fprintf(stderr, "Unknown option <%c>\n", optopt);
                break;
            case ':':
                fprintf(stderr, "Missing argument for option <%c>\n", optopt);
                usage();
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
