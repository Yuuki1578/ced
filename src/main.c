#include <ced/io_fcntl.h>

int main(int argc, char **argv, char **env) {
    IoStream out = io_new(nullptr, KIND_STDOUT);

    io_write(&out, "Hello, ");
    io_write(&out, "world!\n");
    io_flush(&out);

    return 0;
}
