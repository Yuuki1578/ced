// dev only
#define CED_PRIVATE_IMPL

#include <ced/io_fcntl.h>
#include <ced/string.h>
#include <stdio.h>

Reader Stdin;
Writer Stdout;
Writer Stderr;

void global_stream_initialize(void) {
  Stdin = reader_new(stdin);
  Stdout = writer_new(stdout);
  Stderr = writer_new(stderr);
}

Reader reader_new(FILE *istream) {
  Reader reader;

  if (istream == nullptr) {
    reader.istream = stdin;
  }

  else {
    reader.istream = istream;
  }

  reader.buffer = string_new();
  return reader;
}

size_t reader_read(Reader *reader) {
  auto buffer = string_new();
  
  if (reader == nullptr || reader->istream == nullptr) {
    return 0;
  }
}

void reader_close(Reader *reader) {
  if (reader == nullptr) {
    return;
  }

  if (reader->istream == nullptr) {
    goto end_label;
  }

  fclose(reader->istream);

end_label:
  string_dealloc(&reader->buffer);
}

Writer writer_new(FILE *ostream) {
  Writer writer;

  if (ostream == nullptr) {
    writer.ostream = stdout;
  }

  else {
    writer.ostream = ostream;
  }

  writer.buffer = string_new();
  return writer;
}

void writer_close(Writer *writer) {
  if (writer == nullptr) {
    return;
  }

  if (writer->ostream == nullptr) {
    goto end_label;
  }

  fclose(writer->ostream);

end_label:
  string_dealloc(&writer->buffer);
}
