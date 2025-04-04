#include <ced/string.h>
#include <ced/bufio.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    auto buf = bufio_open("src/bufio.c", "r");

    bufio_read_all(&buf);

    printf("%s", buf.buffer.raw_str);

    bufio_clear(&buf);
    bufio_close(&buf);

    return 0;
}
