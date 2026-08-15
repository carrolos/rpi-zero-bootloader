// ripped from engler, cs140e, with minor edits
#include "libunix.h"

void read_check(const uint8_t * const buf, const uint32_t n)
{
    const char *name = "temp.txt";

    int fd = create_file(name);
    if (n)
        write_exact(fd, buf, n);

    uint32_t nbytes;
    uint8_t *data = read_file(&nbytes, name);
    unlink(name);
    assert(data);

    // ensure contents match
    uint32_t i;
    for (i = 0; i < n; i++)
    {
        if (data[i] != buf[i])
        {
            panic("read file doesn't match: %uth run differs at offset %u; \
                expect=<%x>, got=<%x>\n", n, i, buf[i], data[i]);
        }
    }

    // ensure is zero padded
    const uint32_t round = pi_roundup(n, 4);
    for (; i < round; i++)
    {
        if (data[i])
            panic("read_file did not zero pad at offset %u during run %u\n",
                i, n);
    }

    close(fd);
    free(data);
}

int main(void)
{
    const uint32_t N = 512;
    uint8_t *mem = malloc(N);

    for (uint32_t i = 0; i < N; i++)
        mem[i] = random();
    for (uint32_t i = 1; i < N; i++)
        read_check(mem, i);

    output("SUCCESS!\n");
    free(mem);
}
