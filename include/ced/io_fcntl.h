#pragma once

#include <ced/string.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#ifndef CED_IO_FCNTL
#define CED_IO_FCNTL

typedef struct Writer Writer;
typedef struct Reader Reader;

struct Writer {
  FILE *ostream;
  String buffer;
};

struct Reader {
  FILE *istream;
  String buffer;
};

// polymorphistic enum-like File handler struct
// @WARNING:
// there can be only 1 StreamType variant on this struct
// otherwise, the data in the memory for Reader and Writer
// will overlap to each other
struct File {
  enum OpenOption : uint8_t {
    READ_ONLY = 0,
    READ_WRITE = 1,
    WRITE_TRUNCATE = 2,
    WRITE_TRUNCATE_CREATE = 3,
    WRITE_APPEND_CREATE = 4,
  } kind;

  union StreamType {
    Reader istream;
    Writer ostream;
  } stream;
};

// global, staticalliy-lifetimed standard input
extern Reader Stdin;

// global, staticalliy-lifetimed standard output
extern Writer Stdout;

// global, staticalliy-lifetimed standard error
extern Writer Stderr;

#ifdef CED_PRIVATE_IMPL
void global_stream_initialize(void);
#endif

// --- Reader ---
//
// create a new reader to a FILE* instance
Reader reader_new(FILE *istream);

// read from an instance of FILE* to an inner buffer
size_t reader_read(Reader *reader);

// return the inner buffer and closing the FILE* instance
String reader_bufftake(Reader *reader);

// close the FILE* instance, releasing it's buffer if not moved
void reader_close(Reader *reader);

// --- Writer ---
//
// create a new writer to a FILE* instance
Writer writer_new(FILE *ostream);

// write a byte to an inner buffer of Writer
size_t writer_write(Writer *writer, char *buffer);

// flushing the writer, ensuring all the byte reached the FILE* instance
void writer_flush(Writer *writer);

// close the FILE* instance and releasing it's buffer
void writer_close(Writer *writer);

// --- struct File ---
//
// create a new file handler
struct File file_new(const char *path, enum OpenOption opt);

#endif
