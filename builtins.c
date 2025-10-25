#include "shell.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int handle_builtins(vect_t *tokens, const char *prev_input) {
	const char *cmd = vect_get(tokens, 0);

	if (strcmp(cmd, "cd") == 0) {
		const char *path = vect_get(tokens, 1);
		if (path == NULL) {
			fprintf(stderr, "cd: missing argument\n");
		} else if (chdir(path) != 0) {
			perror("cd");
		}
		return 1;
	}

	if (strcmp(cmd, "help") == 0) {
		printf("Built-in commands:\n");
		printf("  cd <path>  Change directory\n");
		printf("  exit       Exit the shell\n");
		printf("  prev       Repeat the last command\n");
		printf("  source <f> Run commands from file\n");
		printf("  help       Show this message\n");
		return 1;
	}

	if (strcmp(cmd, "prev") == 0) {
		// do previous command
		if (strlen(prev_input) == 0) {
			printf("No previous commmand.\n");
		} else {

			printf("%s\n", prev_input);
		}
		return 1;
	}

	if (strcmp(cmd, "source") == 0) {
		// take file and line by line send to shell
		// somehow..?
		return 1;
	}

	// not a built in command
	return 0;
}
