#ifndef SHELL_H
#define SHELL_H

#include "tokenizer/tokens.h"
#include "vector/vect.h"

#define MAX_SIZE 255

void execute_line(const char *line, char *prev_input);

int handle_builtin(vect_t *tokens, const char *prev_input);

void process_command(vect_t *tokens);

#endif
