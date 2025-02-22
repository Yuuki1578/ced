#include <ced/io_fcntl.h>
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
        string_pushstr(&stream->buffer, buffer);
        break;
    
    // do not write to a buffer if stream->kind == KIND_STDIN or above 3
    default:
        return CED_IO_ERROR;
    }

    // success
    return 0;
}

uint64_t io_flush(IoStream *stream) {
    uint64_t retval = 0;

    if (stream == nullptr || stream->stream == nullptr)
        return retval;

    switch (stream->kind) {
    case KIND_STDOUT:
    case KIND_STDERR:
    case KIND_FILESYS:
        if (stream->buffer.raw_str == nullptr)
            return retval;

        retval = fwrite(string(&stream->buffer), stream->buffer.layout.t_size, stream->buffer.len, stream->stream);
        fflush(stream->stream);

    default:
        break;
    }

    return retval;
}
