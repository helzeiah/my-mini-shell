CC=gcc
CFLAGS=-Werror -g -std=gnu11

SHELL_OBJS=shell.o tokenizer/tokens.o vector/vect.o builtins.o process.o

ifeq ($(shell uname), Darwin)
	LEAKTEST ?= leaks --atExit --
else
	LEAKTEST ?= valgrind --leak-check=full
endif

.PHONY: all valgrind clean test

all: shell

valgrind: shell
	$(LEAKTEST) ./shell

shell-tests : %-tests: %
	env python3 tests/$*_tests.py

test: shell-tests 

clean: 
	rm -rf *.o tokenizer/*.o vector/*.o
	rm -f shell

shell: $(SHELL_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

