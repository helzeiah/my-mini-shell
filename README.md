# my-mini-shell

a small unix like shell written in C. Supports command execution, I/O redirection, sequencing and piping. 

---

## features
- run any valid system commanmd (e.g. `ls`, `echo`, `sort`, `cat`)
- a few builtin commands:
-- `cd <path>` - change directory
- `help` - list available built-ins
- `exit` - terminate shell
- `prev` - re-run the last ran command
- `source <file>` - execute commands from a file sequentially

### allows for these tools
- command sequencing (`cmd1; cmd2; cmd3`)
- input/output redirection (`<`, `>`)
- single-level piping (`cmd1 | cmd2`)
- redirection & piping (`sort < file | nl`)

## architecture
| component | description |
|------------|-------------|
| `shell.c` | main loop & command dispatching |
| `builtins.c` | handles built-in shell commands |
| `process.c` | handles process creation, redirection, and pipes |
| `tokenizer/` | converts user input into tokens |
| `vector/` | dynamic array library for tokens |
| `shell.h` | shared header across modules |

## example yay
```bash
./shell
Welcome to mini-shell.
shell $ echo "hello world"
hello world
shell $ sort < words.txt | nl
     1  apple
     2  banana
     3  carrot
shell $ prev
sort < words.txt | nl
     1  apple
     2  banana
     3  carrot
shell $ exit
Bye bye.
```

## design notes from me
- piggybacked off of prev systems assignments - used tokenizer & vector from class
- avoids memory leaks with proper clean up
- could be faster without using dynamic vector
- uses `fork()`, `execvp()`, etc for process management

## how to run
There is a Makefile that allows you to easily run my mini shell
All you really need is
```bash
make all
./shell
```
you can also run:
`make clean`
`make valgrind`


