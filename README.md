# holbertonschool-simple_shell

## Simple Shell

This is a simple UNIX command line interpreter, created as part of a project for Holberton School. The shell supports various features and commands, allowing users to interact with their system through a command-line interface.

---

## Usage

To use the shell, simply run the executable:

./simple_shell

The shell will display a prompt and wait for the user to type a command. A command line always ends with a new line. The prompt is displayed again each time a command has been executed.

---

## Features

   - Display a prompt and wait for user input.
   - Execute simple commands (one word only).
   - Handle errors gracefully, including when an executable cannot be found.
   - Handle the "end of file" condition (Ctrl+D) to exit the shell.

---

## Requirements:

    - Allowed editors: vi, vim, emacs.
    - Compilation: gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh
    - Code style: Follow the Betty style. It will be checked using betty-style.pl and betty-doc.pl.
    - Memory leaks: Your shell should not have any memory leaks.
    - Functions per file: No more than 5 functions per file.
    - Header files: All header files should be include guarded.
    - System calls: Use system calls only when necessary.


### Output:

Unless specified otherwise, your program must have the exact same output as sh (/bin/sh) as well as the exact same error output. The only difference is when you print an error, the name of the program must be equivalent to your argv[0].

### Compilation

Your shell will be compiled this way:
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh

### Testing
Shell should work in both interactive and non-interactive modes.

### Interactive mode:
$ ./hsh

($) /bin/ls

hsh main.c shell.c

($)

($) exit

$

### Non-interactive mode:
$ echo "/bin/ls" | ./hsh

hsh main.c shell.c test_ls_2

$

$ cat test_ls_2

/bin/ls

/bin/ls

$

$ cat test_ls_2 | ./hsh

hsh main.c shell.c test_ls_2

hsh main.c shell.c test_ls_2

$

---

## Version History
### Task

### Simple Shell 0.1

   -  Handle command lines with arguments.

### Simple Shell 0.2

   -  Handle the PATH to locate executables.
   -  Avoid calling fork if the command doesn't exist.

### Simple Shell 0.3

   -  Implement the exit built-in command.
        Usage: exit
        Exits the shell without handling any arguments.

### Simple Shell 0.4

   -  Implement the env built-in command.
        Usage: env
        Prints the current environment.

### Simple Shell 0.4

  - Implement the env built-in, that prints the current environment

---

## Authors

    Yahsai Santana
    Eunielis Serrano
