#include <ced/io.h>
#include <ced/memory.h>
#include <ced/string.h>
#include <memory.h>
#include <string.h>

io_t io_new(FILE *stream, int kind) {
  io_t init;

  // default state
  init.stream = stderr;
  init.kind = STREAM_STDERR;
  init.buffer = str_new();

  switch (kind) {
  case STREAM_STDIN:
    init.stream = stdin;
    init.kind = kind;
    break;

  case STREAM_STDOUT:
    init.stream = stdout;
    init.kind = kind;
    break;

  case STREAM_STDERR:
    init.stream = stderr;
    init.kind = kind;
    break;

  case STREAM_FILE:
    if (stream == nullptr)
      return init;

    init.stream = stream;
    init.kind = kind;
    break;
  }

  return init;
}

int io_write(io_t *stream, char *buffer) {
  if (stream == nullptr || buffer == nullptr)
    return WRITE_FAILURE;

  switch (stream->kind) {
  case STREAM_STDOUT:
  case STREAM_STDERR:
  case STREAM_FILE:

    // save the buffer to the internal buffer (dynamically)
    str_pushstr(&stream->buffer, buffer);
    break;

  // do not write to a buffer if stream->kind == KIND_STDIN or above 3
  default:
    return WRITE_FAILURE;
  }

  // success
  return WRITE_SUCCESS;
}

int io_read(io_t *stream, size_t count) {
  str_t *buffer;

  if (stream == nullptr || !count)
    return READ_FAILURE;

  buffer = &stream->buffer;

  switch (stream->kind) {
  case STREAM_STDIN:
  case STREAM_FILE:
    if (stream->stream == nullptr)
      return READ_FAILURE;

    str_reserve(buffer, count);

    if (buffer->layout.status == null_ptr)
      return ALLOC_FAILURE;

    if (fread(buffer->raw_str, sizeof(char), count, stream->stream) == 0)
      return READ_FAILURE;

    rewind(stream->stream);
    break;

  default:
    return READ_FAILURE;
  }

  // success
  return READ_SUCCESS;
}

int io_read_all(io_t *stream) {
  if (stream == nullptr || stream->stream == nullptr)
    return READ_FAILURE;

  switch (stream->kind) {
  case STREAM_STDIN:
  case STREAM_FILE:

    // cheap 128 byte temporary buffer
    layout_t layout = layout_new(sizeof(char), CED_FILE_BYTES_OFFSET);
    char *heapbuf = layout_alloc(&layout, CED_DEFAULT);

    if (layout.status == null_ptr)
      return ALLOC_FAILURE;

    while ((heapbuf = fgets(heapbuf, CED_FILE_BYTES_OFFSET, stream->stream)) !=
           nullptr) {
      str_pushstr(&stream->buffer, heapbuf);
      memset(heapbuf, 0, CED_FILE_BYTES_OFFSET);
    }

    layout_dealloc(&layout, heapbuf);
    break;

  default:
    return READ_FAILURE;
  }

  // success
  return READ_SUCCESS;
}

int io_read_until(io_t *stream, int delim) {
  if (stream == nullptr || stream->stream == nullptr)
    return READ_FAILURE;

  switch (stream->kind) {
  case STREAM_STDIN:
  case STREAM_FILE:
    str_t *buf = io_buffer(stream);
    int ch;

    while ((ch = fgetc(stream->stream)) != delim && ch != EOF)
      str_push(buf, ch);

    if (stream->kind == STREAM_FILE)
      rewind(stream->stream);

    break;

  default:
    return READ_FAILURE;
  }

  return READ_SUCCESS;
}

int io_flush(io_t *stream) {
  if (stream == nullptr || stream->stream == nullptr)
    return WRITE_FAILURE;

  switch (stream->kind) {
  case STREAM_STDOUT:
  case STREAM_STDERR:
  case STREAM_FILE:
    if (stream->buffer.raw_str == nullptr)
      return WRITE_FAILURE;

    if (fwrite(str(&stream->buffer), stream->buffer.layout.t_size,
               stream->buffer.len, stream->stream) == 0 &&
        fflush(stream->stream) != 0)
      return WRITE_FAILURE;

  default:
    return WRITE_FAILURE;
  }

  // success
  return WRITE_SUCCESS;
}

str_t *io_buffer(io_t *stream) {
  return stream == nullptr ? nullptr : &stream->buffer;
}

str_t io_bufftake(io_t *stream) {
  str_t string = str_new();

  if (stream == nullptr)
    return string;

  string = stream->buffer;
  stream->buffer.raw_str = nullptr;

  io_clear(stream);
  return string;
}

str_t io_buffcopy(io_t *stream) {
  str_t str = str_new();

  if (stream == nullptr) {
    return str;
  }

  str_reserve(&str, stream->buffer.layout.cap);

  if (str.layout.status == null_ptr) {
    return str;
  }

  memcpy(str.raw_str, stream->buffer.raw_str, stream->buffer.len);
  return str;
}

void io_clear(io_t *stream) {
  if (stream == nullptr)
    return;

  str_free(&stream->buffer);
}

void io_close(io_t *stream) {
  if (stream == nullptr)
    return;

  stream->kind = UINT8_MAX;
  io_clear(stream);
}
