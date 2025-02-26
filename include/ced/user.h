#pragma once

#include <ced/io.h>
#include <ced/string.h>
#include <ced/memory.h>
#include <stddef.h>
#include <stdbool.h>

#ifndef CED_USER
#define CED_USER

extern IoStream __ostream;
extern IoStream __istream;
extern IoStream *ostream;
extern IoStream *istream;

void user_prelude(void);
void user_destroy(void);
bool user_has_initialized(void);
int user_input(int delim);
int user_output(char *buffer);

#endif
