#include "shell.h"
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

void execute_line(const char *line, char *prev_input) {
	vect_t * tokens = tokenize(line);
	if (vect_size(tokens) == 0) {
		vect_delete(tokens);
		return;
	}
	int result = handle_builtin(tokens, prev_input);
	switch (result) {
		case 2:
		vect_delete(tokens);
		exit(0);
		case 0:
		process_command(tokens);
		break;
		default:
		break;
	}

	strcpy(prev_input, line);
	vect_delete(tokens);
}

int main(int argc, char **argv) {
	char buffer[MAX_SIZE];
	char prev_input[MAX_SIZE] = "";

	printf("Welcome to mini-shell.\n");
	
	while(1) {
		printf("shell $ ");

		char *input = fgets(buffer, MAX_SIZE, stdin);
		// cntrl-D / input error
		if (input == NULL) {
			break;
		}
		execute_line(input, prev_input);
	}
	printf("Bye bye.\n");
	return 0;
}
