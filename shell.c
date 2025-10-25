#include "tokenizer/tokens.h"
#include "vector/vect.h"
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#define MAX_SIZE 255

int main(int argc, char **argv) {

	printf("Welcome to mini-shell.\n");
	
	while(1) {
		printf("shell $ ");
		char buffer[MAX_SIZE];

		char *input = fgets(buffer, MAX_SIZE, stdin);
		if (input == NULL) {
			printf("Bye bye.\n");
			break;
		}
		vect_t *tokens = tokenize(input);
		if (vect_size(tokens) == 0) {
			vect_delete(tokens);
			continue;
		}
		const char *first = vect_get(tokens, 0);
		if (strcmp(first, "exit") == 0) {
			printf("Bye bye.\n");
			break;
		}
		char *args[vect_size(tokens) + 1];
		for (unsigned int i = 0; i < vect_size(tokens); i++) {
			args[i] = (char *)vect_get(tokens, i);
		}
		args[vect_size(tokens)] = NULL;
		
		pid_t pid = fork();

		if (pid == 0) {
			// child process
			execvp(args[0], args);
			fprintf(stderr, "%s: command not found\n", args[0]);
			exit(1);
		} else {
			// parent process
			waitpid(pid, NULL, 0);
		}
		vect_delete(tokens);
	}

	return 0;
}
