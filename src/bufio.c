#include <ced/bufio.h>
#include <ced/memory.h>
#include <ced/string.h>
#include <sys/types.h>
#include <stddef.h>
#include <string.h>
#include <memory.h>

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

bufio_t bufio_open(const char *path, const char *mode)
{
    return (bufio_t) {
        .stream = fopen(path, mode),
        .kind = STREAM_FILE,
        .buffer = string_new(),
    };
}

ssize_t bufio_write(bufio_t *stream, char *buffer)
{
    ssize_t len;

    if (stream == nullptr || buffer == nullptr)
        return BUFIO_UNSPEC_FAILURE;

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
    return len;
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

        if (buffer->layout.cap <= count)
            string_reserve(buffer, buffer->layout.cap + count);

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

ssize_t bufio_read_all(bufio_t *stream)
{
    ssize_t nbytes = 0;
    ssize_t offset;
    ssize_t current;

    if (stream == nullptr || stream->stream == nullptr)
        return BUFIO_UNSPEC_FAILURE;

    switch (stream->kind) {
    case STREAM_FILE:
        if (stream->stream == nullptr)
            return BUFIO_UNSPEC_FAILURE;

        current = ftell(stream->stream);
        fseek(stream->stream, 0, SEEK_END);
        offset = ftell(stream->stream) + 1;

        if (stream->buffer.layout.cap <= offset)
            string_reserve(&stream->buffer, stream->buffer.layout.cap + offset);

        fseek(stream->stream, current, SEEK_SET);
        fread(stream->buffer.raw_str + stream->buffer.len, sizeof(char), offset, stream->stream);

        break;

    default:
        return BUFIO_UNSPEC_FAILURE;
    }

    // success
    return nbytes;
}

ssize_t bufio_read_until(bufio_t *stream, int delim)
{
    string_t    *buffer;
    int         ch;
    ssize_t     count;

    if (stream == nullptr || stream->stream == nullptr)
        return BUFIO_UNSPEC_FAILURE;

    switch (stream->kind) {
    case STREAM_STDIN:
    case STREAM_FILE:
        buffer = bufio_buffer(stream);

        if (buffer == nullptr)
            return BUFIO_UNSPEC_FAILURE;

        while ((ch = fgetc(stream->stream)) != delim && ch != EOF) {
            string_push(buffer, ch);
            count++;
        }

        break;

    default:
        return BUFIO_UNSPEC_FAILURE;
    }

    return count;
}

ssize_t bufio_flush(bufio_t *stream)
{
    ssize_t written;

    if (stream == nullptr || stream->stream == nullptr)
        return BUFIO_UNSPEC_FAILURE;

    switch (stream->kind) {
    case STREAM_STDOUT:
    case STREAM_STDERR:
    case STREAM_FILE:
        if (stream->buffer.raw_str == nullptr)
            return BUFIO_UNSPEC_FAILURE;

        if ((written = fwrite(STR(stream->buffer), stream->buffer.layout.t_size, stream->buffer.len, stream->stream)) == 0 && fflush(stream->stream) != 0)
            return BUFIO_WRITE_FAILURE;

    default:
        return BUFIO_UNSPEC_FAILURE;
    }

    // success
    return written;
}

string_t *bufio_buffer(bufio_t *stream)
{
    return stream == nullptr ? nullptr : &stream->buffer;
}

string_t bufio_buffer_move(bufio_t *stream)
{
    string_t string = string_new();

    if (stream == nullptr)
        return string;

    string = stream->buffer;
    stream->buffer.raw_str = nullptr;
    bufio_clear(stream);

    return string;
}

string_t bufio_buffer_copy(bufio_t *stream)
{
    string_t str = string_new();

    if (stream == nullptr)
        return str;

    string_reserve(&str, stream->buffer.layout.cap);

    if (str.layout.status == NULL_PTR)
        return str;

    string_pushstr(&str, stream->buffer.raw_str);
    return str;
}

void bufio_clear(bufio_t *stream)
{
    if (stream == nullptr)
        return;

    string_free(&stream->buffer);
}

void bufio_close(bufio_t *stream)
{
    if (stream == nullptr)
        return;

    bufio_clear(stream);
    
    if (stream->kind == STREAM_FILE)
        fclose(stream->stream);

    *stream = bufio_new(nullptr, STREAM_STDERR);
}
