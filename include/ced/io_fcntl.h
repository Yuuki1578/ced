#pragma once

#include <ced/string.h>
#include <stdint.h>
#include <stdio.h>

#ifndef CED_IO_FCNTL
#define CED_IO_FCNTL

#define CED_IO_ERROR (-1);
#define CED_FILE_BYTES_OFFSET CHAR_MAX

typedef struct IoStream IoStream;

// buffered I/O handler
struct IoStream {
    FILE *stream;

    enum StreamKind : uint8_t {
        KIND_STDIN = 0,
        KIND_STDOUT = 1,
        KIND_STDERR = 2,
        KIND_FILESYS = 3,
    } kind;

    String buffer;
};

// bound a FILE*
IoStream io_new(FILE *stream, enum StreamKind kind);

// write a bytes to a buffer, return -1 on error
int8_t io_write(IoStream *stream, char *buffer);

// flush an opened stream, ensuring all bytes reached it's destination
// return 0 on success, -1 on failure
int8_t io_flush(IoStream *stream);

// truncating it's buffer to zero or nullptr
void io_clear(IoStream *stream);

// read <nth> bytes from opened stream, store it in the buffer
// after completed, the stream position is set to the beginning again
int8_t io_read(IoStream *stream, uint64_t nth);

// read all bytes, contained in opened stream, and store it in the buffer
int8_t io_read_all(IoStream *stream);

// return a pointer to a String buffer
String *io_buffer(IoStream *stream);

// move a buffer, note that the buffer from IoStream *stream is moved as a return value
String io_bufftake(IoStream *stream);

// copying a buffer from IoStream *stream, return it's copy
String io_buffcopy(IoStream *stream);

// close a bound to FILE*, releasing it's buffer
// note that if the StreamKind is KIND_FILESYS
// you still need to close the FILE* using fclose()
void io_close(IoStream *stream);

#endif
