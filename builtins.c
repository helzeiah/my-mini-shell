#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
 * @brief Handles all supported builtin commands for the minishell
 * (cd, help, prev, source, exit).
 *
 * @param tokens a vect_t pointer containing string tokens
 * @param prev_input a string containing the previous input
 *
 * @return 2 if cmd was 'exit',
 *         1 if given cmd was a builtin cmd,
 *         0 otherwise.
 */
int handle_builtins(vect_t *tokens, char *prev_input) {
  const char *cmd = vect_get(tokens, 0);

  if (strcmp(cmd, "exit") == 0) {
    return 2;
  }

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

      char temp[MAX_SIZE];
      strcpy(temp, prev_input);
      execute_line(temp, prev_input);
    }
    return 1;
  }

  if (strcmp(cmd, "source") == 0) {
    const char *filename = vect_get(tokens, 1);
    if (!filename) {
      fprintf(stderr, "source: missing argument\n");
      return 1;
    }

    FILE *file = fopen(filename, "r");
    if (!file) {
      perror("source");
      return 1;
    }

    char line[MAX_SIZE];
    while (fgets(line, MAX_SIZE, file)) {
      execute_line(line, prev_input);
    }

    fclose(file);
    return 1;
  }

  // not a built in command
  return 0;
}
