#ifndef CED_BUFIO_H
#define CED_BUFIO_H

#include <ced/string.h>
#include <sys/types.h>
#include <stddef.h>
#include <stdio.h>

// ssize_t -> <sys/types.h>
#define BUFIO_OP_SUCCESS        0LL
#define BUFIO_UNSPEC_FAILURE    -1LL
#define BUFIO_READ_FAILURE      -2LL
#define BUFIO_WRITE_FAILURE     -3LL

#define STREAM_STDIN            0
#define STREAM_STDOUT           1
#define STREAM_STDERR           2
#define STREAM_FILE             3

/*
 * buffered I/O handler
 * */
typedef struct {
    FILE        *stream;
    int         kind;
    string_t    buffer;
} bufio_t;

/*
 * Bound instance to a FILE*
 * */
bufio_t bufio_new(FILE *stream, int kind);

/*
 * write a bytes to a buffer, return -1 on error
 * */
ssize_t bufio_write(bufio_t *stream, char *buffer);

/*
 * flush the opened stream, ensuring all bytes reached it's destination
 * return 0 on success, -1 on failure
 * */
ssize_t bufio_flush(bufio_t *stream);

/*
 * truncating it's buffer to zero or nullptr
 * */
void bufio_buffer_clear(bufio_t *stream);

/*
 * read count bytes from the opened stream, store it in the buffer
 * after completed, the stream position is set to the beginning again
 * */
ssize_t bufio_read(bufio_t *stream, size_t count);

/*
 * read all bytes from opened stream, and store it in the buffer
 * */
ssize_t bufio_read_all(bufio_t *stream);

/*
 * read all bytes until delim occured
 * TODO: check if delim is in ASCII boundary
 * */
ssize_t bufio_read_until(bufio_t *stream, int delim);

/*
 * return a pointer to an inner buffer
 * */
string_t *bufio_buffer(bufio_t *stream);

/*
 * move a buffer and return it, buffer is moved from
 * stream is moved as a return value
 * */
string_t bufio_buffer_move(bufio_t *stream);

/*
 * copy a buffer from stream, return it's copy
 * */
string_t bufio_buffer_copy(bufio_t *stream);

/*
 * close a bound to FILE*, releasing it's buffer
 * and closing the stream using fclose() if stream->kind == STREAM_FILE
 * */
void bufio_close(bufio_t *stream);

/*
 * truncating the buffer from stream to null
 * */
void bufio_clear(bufio_t *stream);

#endif
