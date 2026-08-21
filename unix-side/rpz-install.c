// inspired by Dawson Engler's bootloader for CS140E loads a program to a rpi
// zero connected via a tty-USB device

#include <stdio.h>
#include <getopt.h>

#include "libunix.h"

static int f_verbose;
static int f_help;

static struct option long_options[] =
{
    {"baud-rate", required_argument, NULL,     'b'},
    {"last",      no_argument,       NULL,     'l'},
    {"first",     no_argument,       NULL,     'f'},
    {"device",    required_argument, NULL,     'd'},
    {"verbose",   no_argument,       &f_verbose, 1},
    {"help",      no_argument,       &f_help,    1},
    // sentinel
    {NULL,        0,                 NULL,      0 }
};

static char *this_prog;

// prints a usage string; called when the user requests for help or gives bad
// options
static void usage()
{
    output("\nusage: %s [(-b | --baud_rate) <baud>] [(-l | --last) | (-f | "
        "--first) | (-d | --device) </path/to/device>] [-v | --verbose] "
        "<pi_program>\n",
        this_prog);
}

struct rate_to_constant
{
    int rate;
    speed_t constant;
};

struct rate_to_constant baud_map[] =
{
    {9600, B9600},
    {19200, B19200},
    {38400, B38400},
    {57600, B57600},
    {115200, B115200},
    {230400, B230400},
    // sentinel
    {0, 0}
};


// returns the corresponding baud rate constant if the passed-in rate is
// accepted; otherwise returns 0
//
// accepts a subset of those constants defined in termios.h
static int check_baud(int b)
{
    struct rate_to_constant *p = &baud_map[0];
    while (p->rate)
    {
        if (b == p->rate)
            return p->constant;
        p++;
    }

    return 0;
}

int main(int argc, char ** argv)
{
    this_prog = argv[0];
    uint32_t baud = B115200;
    // default behavior uses most recently plugged-in device
    // options that modify dev_name overwrite each other
    char * dev_name = find_ttyusb_last();
    char * pi_prog = "";

    int opt_index;
    int c;
    while (1)
    {
        // the leading '-' tells getopt to not reorder argv
        // the ':' after the leading '-' tells getopt that we'll handle errors
        c = getopt_long(argc, argv, "-:b:lfd:vh", long_options, &opt_index); 
        if (c == -1)
            break;
        switch (c)
        {
            case 0:
                // got a long option that is either verbose or help; handled
                // by corresponding global flag already, so do nothing here
                break;
            case 1:
                if (pi_prog[0] == '\0')
                    pi_prog = optarg;
                else
                {
                    printf("too many non-option pi programs; bootloader takes"
                        " only one pi program\n");
                    usage();
                    exit(1);
                }
                break;
            case 'b':
                baud = check_baud(atoi(optarg));
                if (!baud)
                {
                    fprintf(stderr, "bad baud rate <%d>; pass a valid baud "
                        "rate", atoi(optarg));
                    usage();
                    exit(1);
                }
                break;
            case 'l':
                dev_name = find_ttyusb_last();
                break;
            case 'f':
                dev_name = find_ttyusb_first();
                break;
            case 'd':
                // dies if the specified device is bogus
                find_ttyusb(optarg);
                dev_name = optarg;
                break;
            // consider editing array entry because decoupled; seems simple
            // enough to not cause problems, however
            case 'v':
                f_verbose = 1;
                break;
            case 'h':
                f_help = 1;
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

    if (f_help)
    {
        usage();
        exit(0);
    }

    // shouldn't be empty at this point
    if (pi_prog[0] == '\0')
    {
        panic("Did not receive a pi program to bootload; exactly one is required");
        usage();
    }
    if (dev_name[0] == '\0')
        panic("All options parsed, yet tty device is empty! [is one plugged"
            " in?]\n");

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
