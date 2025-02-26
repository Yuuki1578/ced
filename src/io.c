#include <ced/memory.h>
#include <ced/string.h>
#include <ced/io.h>
#include <memory.h>
#include <string.h>

IoStream io_new(FILE *stream, int kind) {
    IoStream init;

    // default state
    init.stream = stderr;
    init.kind = STREAM_STDERR;
    init.buffer = string_new();

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

int io_write(IoStream *stream, char *buffer) {
    if (stream == nullptr || buffer == nullptr)
        return WRITE_FAILURE;

    switch (stream->kind) {
    case STREAM_STDOUT:
    case STREAM_STDERR:
    case STREAM_FILE:
        
        // save the buffer to the internal buffer (dynamically)
        string_pushstr(&stream->buffer, buffer);
        break;
    
    // do not write to a buffer if stream->kind == KIND_STDIN or above 3
    default:
        return WRITE_FAILURE;
    }

    // success
    return WRITE_SUCCESS;
}

int io_read(IoStream *stream, size_t count) {
    String *buffer;

    if (stream == nullptr || !count)
        return READ_FAILURE;

    buffer = &stream->buffer;

    switch (stream->kind) {
    case STREAM_STDIN:
    case STREAM_FILE:
        if (stream->stream == nullptr)
            return READ_FAILURE;

        string_reserve(buffer, count);

        if (buffer->layout.status == NULLPTR)
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

int io_read_all(IoStream *stream) {
    if (stream == nullptr || stream->stream == nullptr)
        return READ_FAILURE;

    switch (stream->kind) {
    case STREAM_STDIN:
    case STREAM_FILE:

        // cheap 128 byte temporary buffer
        Layout layout = layout_new(sizeof(char), CED_FILE_BYTES_OFFSET);
        char *heapbuf = layout_alloc(&layout, CED_DEFAULT);

        if (layout.status == NULLPTR)
            return ALLOC_FAILURE;

        while ((heapbuf = fgets(heapbuf, CED_FILE_BYTES_OFFSET, stream->stream)) != nullptr) {
            string_pushstr(&stream->buffer, heapbuf);
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

int io_read_until(IoStream *stream, int delim) {
    if (stream == nullptr || stream->stream == nullptr)
        return READ_FAILURE;

    switch (stream->kind) {
    case STREAM_STDIN:
    case STREAM_FILE:
        String *buf = io_buffer(stream);
        int ch;

        while ((ch = fgetc(stream->stream)) != delim && ch != EOF)
            string_push(buf, ch);

        if (stream->kind == STREAM_FILE)
            rewind(stream->stream);

        break;

    default:
        return READ_FAILURE;
    }

    return READ_SUCCESS;
}

int io_flush(IoStream *stream) {
    if (stream == nullptr || stream->stream == nullptr)
        return WRITE_FAILURE;

    switch (stream->kind) {
    case STREAM_STDOUT:
    case STREAM_STDERR:
    case STREAM_FILE:
        if (stream->buffer.raw_str == nullptr)
            return WRITE_FAILURE;

        if (fwrite(string(&stream->buffer), stream->buffer.layout.t_size, stream->buffer.len, stream->stream) == 0 && fflush(stream->stream) != 0)
            return WRITE_FAILURE;

    default:
        return WRITE_FAILURE;
    }

    // success
    return WRITE_SUCCESS;
}

String *io_buffer(IoStream *stream) {
    return stream == nullptr ? nullptr : &stream->buffer;
}

String io_bufftake(IoStream *stream) {
    String string = string_new();

    if (stream == nullptr)
        return string;
    
    string = stream->buffer;
    stream->buffer.raw_str = nullptr;

    io_clear(stream);
    return string;
}

void io_clear(IoStream *stream) {
    if (stream == nullptr)
        return;

    string_dealloc(&stream->buffer);
}

void io_close(IoStream *stream) {
    if (stream == nullptr)
        return;

    stream->kind = UINT8_MAX;
    io_clear(stream);
}
