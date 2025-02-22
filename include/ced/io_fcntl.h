#pragma once

#include <ced/string.h>
#include <stdint.h>
#include <stdio.h>

#ifndef CED_IO_FCNTL
#define CED_IO_FCNTL

#define CED_IO_ERROR (-1);

typedef struct IoStream IoStream;

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

IoStream io_new(FILE *stream, enum StreamKind kind);
int8_t io_write(IoStream *stream, char *buffer);
uint64_t io_flush(IoStream *stream);
int64_t io_read(IoStream *stream, uint64_t nth);
int64_t io_read_all(IoStream *stream);

#endif
