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

