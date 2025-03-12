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
typedef struct {
  FILE *stream;
  int kind;
  str_t buffer;
} io_t;

/*
 * Bound instance to a FILE*
 * */
io_t io_new(FILE *stream, int kind);

/*
 * write a bytes to a buffer, return -1 on error
 * */
int io_write(io_t *stream, char *buffer);

/*
 * flush the opened stream, ensuring all bytes reached it's destination
 * return 0 on success, -1 on failure
 * */
int io_flush(io_t *stream);

/*
 * truncating it's buffer to zero or nullptr
 * */
void io_clear(io_t *stream);

/*
 * read count bytes from the opened stream, store it in the buffer
 * after completed, the stream position is set to the beginning again
 * */
int io_read(io_t *stream, size_t count);

/*
 * read all bytes from opened stream, and store it in the buffer
 * */
int io_read_all(io_t *stream);

/*
 * read all bytes until delim occured
 * TODO: check if delim is in ASCII boundary
 * */
int io_read_until(io_t *stream, int delim);

/*
 * return a pointer to an inner buffer
 * */
str_t *io_buffer(io_t *stream);

/*
 * move a buffer and return it, buffer is moved from
 * stream is moved as a return value
 * */
str_t io_bufftake(io_t *stream);

/*
 * copy a buffer from stream, return it's copy
 * */
str_t io_buffcopy(io_t *stream);

/*
 * close a bound to FILE*, releasing it's buffer
 * note that if the StreamKind is KIND_FILESYS
 * you still need to close the FILE* using fclose()
 * */
void io_close(io_t *stream);

#endif
