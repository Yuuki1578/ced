#include <ced/io.h>
#include <ced/string.h>
#include <stdio.h>

int main(void) {
    FILE *file = fopen(__FILE__, "r");
    IoStream io = io_new(file, KIND_FILESYS);

    io_read_all(&io);

    String str = io_bufftake(&io);
    printf("%s", string(&str));

    string_dealloc(&str);
    io_close(&io);

    return 0;
}
