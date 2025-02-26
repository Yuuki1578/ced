#pragma once

#include <ced/string.h>
#include <stddef.h>
#include <stdio.h>

#ifndef CED_IO_FCNTL
#define CED_IO_FCNTL

#define CED_FILE_BYTES_OFFSET CHAR_MAX

#define WRITE_SUCCESS 0
#define WRITE_FAILURE -1
#define READ_SUCCESS 0
#define READ_FAILURE -1

#define STREAM_STDIN 0
#define STREAM_STDOUT 1
#define STREAM_STDERR 2
#define STREAM_FILE 3

/* 
 * buffered I/O handler
 * */
typedef struct IoStream {
    FILE *stream;
    int kind;
    String buffer;
} IoStream;

/*
 * Bound instance to a FILE*
 * */
IoStream io_new(FILE *stream, int kind);

/* 
 * write a bytes to a buffer, return -1 on error
 * */
int io_write(IoStream *stream, char *buffer);

/* 
 * flush the opened stream, ensuring all bytes reached it's destination
 * return 0 on success, -1 on failure
 * */
int io_flush(IoStream *stream);

/* 
 * truncating it's buffer to zero or nullptr
 * */
void io_clear(IoStream *stream);

/* 
 * read count bytes from the opened stream, store it in the buffer
 * after completed, the stream position is set to the beginning again
 * */
int io_read(IoStream *stream, size_t count);

/*
 * read all bytes from opened stream, and store it in the buffer
 * */
int io_read_all(IoStream *stream);

/*
 * read all bytes until delim occured
 * TODO: check if delim is in ASCII boundary
 * */
int io_read_until(IoStream *stream, int delim);

/*
 * return a pointer to an inner buffer
 * */
String *io_buffer(IoStream *stream);

/* 
 * move a buffer and return it, buffer is moved from
 * stream is moved as a return value
 * */
String io_bufftake(IoStream *stream);

/* 
 * copy a buffer from stream, return it's copy
 * */
String io_buffcopy(IoStream *stream);

/*
 * close a bound to FILE*, releasing it's buffer
 * note that if the StreamKind is KIND_FILESYS
 * you still need to close the FILE* using fclose()
 * */
void io_close(IoStream *stream);

#endif
