#include <ced/io_fcntl.h>
#include <stdio.h>

int main(int argc, char **argv, char **env) {
    FILE *file = fopen(__FILE__, "r");
    IoStream input = io_new(file, KIND_FILESYS);

    io_read(&input, 64);
    io_read(&input, 64);

    printf("%s\n", string(&input.buffer));
    printf("%li\n", input.buffer.layout.cap);

    io_close(&input);

    return 0;
}
