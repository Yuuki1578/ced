#include <ced/bufio.h>
#include <ced/memory.h>
#include <ced/string.h>
#include <memory.h>
#include <string.h>

bufio_t bufio_new(FILE *stream, int kind)
{
    bufio_t init = {
        .stream = stderr,
        .kind = STREAM_STDERR,
        .buffer = string_new(),
    };

    switch (kind) {
    case STREAM_STDIN:
        init.stream = stdin;
        break;

    case STREAM_STDOUT:
        init.stream = stdout;
        break;

    case STREAM_STDERR:
        init.stream = stderr;
        break;

    case STREAM_FILE:
        if (stream == nullptr)
            return init;

        init.stream = stream;
        break;

    default:
        init.kind = STREAM_STDERR;
        return init;
    }

    init.kind = kind;
    return init;
}

ssize_t bufio_write(bufio_t *stream, char *buffer)
{
    size_t len;

    if (stream == nullptr || buffer == nullptr)
        return BUFIO_FAILURE;

    len = strlen(buffer);

    switch (stream->kind) {
    case STREAM_STDOUT:
    case STREAM_STDERR:
    case STREAM_FILE:

        // save the buffer to the internal buffer (dynamically)
        string_pushstr(&stream->buffer, buffer);
        break;

    // do not write to a buffer if stream->kind == KIND_STDIN or above 3
    default:
        return BUFIO_UNSPEC_FAILURE;
    }

    // success
    return (ssize_t) len;
}

ssize_t bufio_read(bufio_t *stream, size_t count)
{
    string_t *buffer;

    if (stream == nullptr || count == 0)
        return BUFIO_UNSPEC_FAILURE;

    buffer = &stream->buffer;

    switch (stream->kind) {
    case STREAM_STDIN:
    case STREAM_FILE:
        if (stream->stream == nullptr)
            return BUFIO_UNSPEC_FAILURE;

        string_reserve(&stream->buffer, stream->buffer.layout.cap + count);

        if (buffer->layout.status == NULL_PTR)
            return BUFIO_UNSPEC_FAILURE;

        if (fread(buffer->raw_str, sizeof(char), count, stream->stream) == 0)
            return BUFIO_READ_FAILURE;

        break;

    default:
        return BUFIO_UNSPEC_FAILURE;
    }

    // success
    return (ssize_t) count;
}

ssize_t bufio_read_all(bufio_t *stream) {
    if (stream == nullptr || stream->stream == nullptr)
        return BUFIO_UNSPEC_FAILURE;

    switch (stream->kind) {
    case STREAM_STDIN:
    case STREAM_FILE:

        // cheap 128 byte temporary buffer
        layout_t layout = layout_new(sizeof(char), CED_STRING_STEP);
        char *heapbuf = layout_alloc(&layout);

        if (layout.status == NULL_PTR)
            return BUFIO_UNSPEC_FAILURE;

        while (fgets(heapbuf, CED_STRING_STEP, stream->stream) != nullptr) {
            string_pushstr(&stream->buffer, heapbuf);
            memset(heapbuf, 0, CED_STRING_STEP);
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
