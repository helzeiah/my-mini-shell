#include "shell.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/**
 * @brief executes a command , supports out & in redirection and piping.
 *
 * @param tokens a vect_t pointer containing string tokens.
 */
void process_command(vect_t *tokens) {
  // check if there's a pipe in the tokens
  int pipe_index = -1;
  for (unsigned int i = 0; i < vect_size(tokens); i++) {
    if (strcmp(vect_get(tokens, i), "|") == 0) {
      pipe_index = i;
      break;
    }
  }

  if (pipe_index != -1) {
    int fds[2];
    if (pipe(fds) == -1) {
      perror("pipe");
      return;
    }

    pid_t pid1 = fork();
    if (pid1 == 0) {
      // first child - left side of pipe
      close(fds[0]); // close unused read end
      dup2(fds[1], STDOUT_FILENO);
      close(fds[1]);

      // check for input redirection in left side
      const char *infile = NULL;
      for (int i = 0; i < pipe_index; i++) {
        if (strcmp(vect_get(tokens, i), "<") == 0 && i + 1 < pipe_index) {
          infile = vect_get(tokens, i + 1);
          break;
        }
      }

      if (infile) {
        int fd = open(infile, O_RDONLY);
        if (fd < 0) {
          perror("open");
          exit(1);
        }
        dup2(fd, STDIN_FILENO);
        close(fd);
      }

      // build args for left command (skip < and filename)
      char *args[pipe_index + 1];
      int j = 0;
      for (int i = 0; i < pipe_index; i++) {
        const char *tok = vect_get(tokens, i);
        if (strcmp(tok, "<") == 0) {
          i++; // skip filename too
          continue;
        }
        args[j++] = (char *)tok;
      }
      args[j] = NULL;

      // execute left command
      execvp(args[0], args);
      fprintf(stderr, "%s: command not found\n", args[0]);
      exit(1);
    }

    pid_t pid2 = fork();
    if (pid2 == 0) {
      // second child - right side of pipe
      close(fds[1]); // close unused write end
      dup2(fds[0], STDIN_FILENO);
      close(fds[0]);

      // right command: tokens[pipe_index+1 .. end]
      unsigned int right_len = vect_size(tokens) - pipe_index - 1;
      char *args[right_len + 1];
      for (unsigned int i = 0; i < right_len; i++) {
        args[i] = (char *)vect_get(tokens, pipe_index + 1 + i);
      }
      args[right_len] = NULL;

      execvp(args[0], args);
      fprintf(stderr, "%s: command not found\n", args[0]);
      exit(1);
    }

    // parent
    close(fds[0]);
    close(fds[1]);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    return;
  }

  const char *infile = NULL;
  const char *outfile = NULL;

  // scan for < and >
  for (unsigned int i = 0; i < vect_size(tokens); i++) {
    const char *tok = vect_get(tokens, i);
    if (strcmp(tok, "<") == 0 && i + 1 < vect_size(tokens)) {
      infile = vect_get(tokens, i + 1);
    } else if (strcmp(tok, ">") == 0 && i + 1 < vect_size(tokens)) {
      outfile = vect_get(tokens, i + 1);
    }
  }

  // build argv (skip <, >, filenames)
  char *args[vect_size(tokens) + 1];
  int j = 0;
  for (unsigned int i = 0; i < vect_size(tokens); i++) {
    const char *tok = vect_get(tokens, i);
    if (strcmp(tok, "<") == 0 || strcmp(tok, ">") == 0) {
      i++; // skip filename too
      continue;
    }
    args[j++] = (char *)tok;
  }
  args[j] = NULL;

  pid_t pid = fork();
  if (pid < 0) {
    perror("fork");
    return;
  }

  if (pid == 0) {
    // child
    if (infile) {
      int fd = open(infile, O_RDONLY);
      if (fd < 0) {
        perror("open");
        exit(1);
      }
      dup2(fd, STDIN_FILENO);
      close(fd);
    }

    if (outfile) {
      int fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
      if (fd < 0) {
        perror("open");
        exit(1);
      }
      dup2(fd, STDOUT_FILENO);
      close(fd);
    }

    execvp(args[0], args);
    fprintf(stderr, "%s: command not found\n", args[0]);
    exit(1);
  } else {
    waitpid(pid, NULL, 0);
  }
}
