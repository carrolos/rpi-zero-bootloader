// inspired by engler, cs140e
#include <sys/stat.h>
#include <dirent.h>
static const char *ttyusb_prefixes[] = {
    "ttyUSB",	// linux
    "ttyACM",   // linux
    "cu.SLAB_USB", // mac os
    "cu.usbserial", // mac os
    "tty.usbserial" //mac os
    // if your system uses another name, add it.
};


#include "libunix.h"

static const char* dev_prefix = "/dev/";
static const uint32_t MAX_LEN = 128;

// scan through the prefixes; returns 1 when a match is found,
// 0 if there is no match.
// MAYBE: will looking through only the prefixes based on the OS
// improve performance?
static int filter(const struct dirent *d)
{
    const uint32_t elen = strlen(d->d_name);
    for (uint32_t i = 0;
        i < (sizeof(ttyusb_prefixes) / sizeof(ttyusb_prefixes[0]));
        i++)
        {
            if (prefix_cmp(d->d_name, ttyusb_prefixes[i]))
                return 1;
        }
    return 0;
}

// compares two struct dirent based on modification time returned by
// stat(); the chosen one is the one that was modified most recently,
// so has the largest `.st_mtime`
static int compar_last(const struct dirent **a, const struct dirent **b)
{
    // assumes full path < 128 bytes
    char buf_a[MAX_LEN];
    char buf_b[MAX_LEN];

    strcpy(buf_a, dev_prefix);
    strcat(buf_a, (*a)->d_name);
    struct stat statbuf_a;
    const int a_err = stat(buf_a, &statbuf_a);
    if (a_err == -1)
        sys_die(stat, "failed when calling `stat` on dirent %s",
            (*a)->d_name);

    strcpy(buf_b, dev_prefix);
    strcat(buf_b, (*b)->d_name);
    struct stat statbuf_b;
    const int b_err = stat(buf_b, &statbuf_b);
    if (b_err == -1)
        sys_die(stat, "failed when calling `stat` on dirent %s",
            (*b)->d_name);

    if (statbuf_a.st_mtime > statbuf_b.st_mtime)
        return -1;
    else
        return 1;
}

static char* find_ttyusb_helper(int last)
{
    struct dirent ** namelist = NULL;
    const int n_entries = scandir(dev_prefix, &namelist, &filter, &compar_last);
    if (n_entries == -1)
        sys_die(scandir, "scandir failed!\n");
    else if (!(n_entries))
        // let the caller handle
        return strdup("");

    // assumes limits on length of device name
    char* ret = malloc(MAX_LEN);
    strcpy(ret, dev_prefix);
    char* end = ret + strlen(dev_prefix);
    if (last)
        // get first entry since last is first, by `compar_last()`
        strcpy(end, namelist[0]->d_name);
    else
        // get last entry since last is first, by `compar_last()`
        strcpy(end, namelist[n_entries - 1]->d_name);
    debug("have device=<%s> with length=<%lu>; verify this\n",
        ret, strlen(ret));
    return ret;
}

// returns the most recently mounted ttyusb (the one
// mounted last).  uses the modification time 
// returned by stat()
char *find_ttyusb_last(void)
{
    return find_ttyusb_helper(1);
}

// returns the oldest mounted ttyusb (the one mounted
// "first") --- uses the modification returned by
// stat()
char *find_ttyusb_first(void)
{
    return find_ttyusb_helper(0);
}
