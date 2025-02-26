#include <ced/io.h>
#include <ced/user.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

IoStream __ostream;
IoStream __istream;
IoStream *ostream;
IoStream *istream;

void user_prelude(void) {
    __ostream = io_new(nullptr, STREAM_STDOUT);
    __istream = io_new(nullptr, STREAM_STDIN);
    ostream = &__ostream;
    istream = &__istream;
}

void user_destroy(void) {
    io_close(ostream);
    io_close(istream);
}

bool user_has_initialized(void) {
    return ostream == nullptr || istream == nullptr ? false : true;
}

int user_input(int delim) {
    return io_read_until(istream, delim);
}
