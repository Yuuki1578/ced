#include <ced/memory.h>
#include <ced/string.h>
#include <ced/io_fcntl.h>
#include <memory.h>
#include <string.h>

IoStream io_new(FILE *stream, enum StreamKind kind) {
    IoStream init;

    // default state
    init.stream = stderr;
    init.kind = KIND_STDERR;
    init.buffer = string_new();

    switch (kind) {
    case KIND_STDIN:
        init.stream = stdin;
        init.kind = kind;
        break;

    case KIND_STDOUT:
        init.stream = stdout;
        init.kind = kind;
        break;

    case KIND_STDERR:
        init.stream = stderr;
        init.kind = kind;
        break;

    case KIND_FILESYS:
        if (stream == nullptr)
            return init;

        init.stream = stream;
        init.kind = kind;
        break;
    }

    return init;
}

int8_t io_write(IoStream *stream, char *buffer) {
    if (stream == nullptr || buffer == nullptr)
        return CED_IO_ERROR;

    if (stream->stream == nullptr)
        return CED_IO_ERROR;

    switch (stream->kind) {
    case KIND_STDOUT:
    case KIND_STDERR:
    case KIND_FILESYS:
        
        // save the buffer to the internal buffer (dynamically)
        string_pushstr(&stream->buffer, buffer);
        break;
    
    // do not write to a buffer if stream->kind == KIND_STDIN or above 3
    default:
        return CED_IO_ERROR;
    }

    // success
    return 0;
}

int8_t io_read(IoStream *stream, size_t count) {
    String *buffer;

    if (stream == nullptr || !count)
        return CED_IO_ERROR;

    buffer = &stream->buffer;

    switch (stream->kind) {
    case KIND_STDIN:
    case KIND_FILESYS:
        if (stream->stream == nullptr)
            return CED_IO_ERROR;

        string_reserve(buffer, count);

        if (buffer->layout.status == NULLPTR)
            return CED_IO_ERROR;

        if (fread(buffer->raw_str, sizeof(char), count, stream->stream) == 0)
            return CED_IO_ERROR;

        rewind(stream->stream);

        break;

    default:
    }

    // success
    return 0;
}

int8_t io_read_all(IoStream *stream) {
    if (stream == nullptr || stream->stream == nullptr)
        return CED_IO_ERROR;

    switch (stream->kind) {
    case KIND_STDIN:
    case KIND_FILESYS:

        // cheap 128 byte temporary buffer
        Layout layout = layout_new(sizeof(char), CED_FILE_BYTES_OFFSET);
        char *heapbuf = layout_alloc(&layout, CED_DEFAULT);

        if (layout.status == NULLPTR)
            return CED_IO_ERROR;

        while ((heapbuf = fgets(heapbuf, CED_FILE_BYTES_OFFSET, stream->stream)) != nullptr) {
            string_pushstr(&stream->buffer, heapbuf);
            memset(heapbuf, 0, CED_FILE_BYTES_OFFSET);
        }

        layout_dealloc(&layout, heapbuf);

        break;

    default:
    }

    // success
    return 0;
}

int8_t io_flush(IoStream *stream) {
    if (stream == nullptr || stream->stream == nullptr)
        return CED_IO_ERROR;

    switch (stream->kind) {
    case KIND_STDOUT:
    case KIND_STDERR:
    case KIND_FILESYS:
        if (stream->buffer.raw_str == nullptr)
            return CED_IO_ERROR;

        if (fwrite(string(&stream->buffer), stream->buffer.layout.t_size, stream->buffer.len, stream->stream) == 0 && fflush(stream->stream) != 0)
            return CED_IO_ERROR;

    default:
        break;
    }

    // success
    return 0;
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
