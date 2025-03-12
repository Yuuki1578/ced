// #include <stdio.h>
// #include <string.h>

// #ifndef CED_VERSION
// #define CED_VERSION 0.01F
// #endif

// #define strequal(lhs, rhs) (!strcmp(lhs, rhs))

// #define ARGS_STDIN 0
// #define ARGS_WRITE 1
// #define ARGS_HELP 2
// #define ARGS_VERSION 3

// int tokenize_args(int argc, char **argv) {
//   char *current;

//   argc--;
//   argv++;

//   if (argc == 0)
//     return ARGS_STDIN;

//   current = *argv;

//   if (strequal(current, "-h") || strequal(current, "--help"))
//     return ARGS_HELP;

//   if (strequal(current, "-v") || strequal(current, "--version"))
//     return ARGS_VERSION;

//   return ARGS_WRITE;
// }

// int main(int argc, char **argv) {
//   int args = tokenize_args(argc, argv);

//   switch (args) {
//   case ARGS_STDIN:
//     printf("From stdin\n");
//     break;

//   case ARGS_WRITE:
//     printf("Write to %s\n", argv[1]);
//     break;

//   case ARGS_HELP:
//     printf("You want some help?\n");
//     break;

//   case ARGS_VERSION:
//     printf("Ced version %.2f\n", CED_VERSION);
//     break;

//   default:
//   }

//   return 0;
// }
#include <ced/io.h>
#include <ced/string.h>

int main(void) {
  FILE *file = fopen(__FILE__, "r");

  if (file == nullptr) {
    return 1;
  }
  
  io_t io = io_new(file, STREAM_FILE);
  io_read_all(&io);
  
  str_t string = io_buffcopy(&io);

  printf("%s", str(&string));

  str_free(&string);
  io_close(&io);
  fclose(file);

  return 0;
}
