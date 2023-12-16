# holbertonschool-simple_shell

## Simple Shell

This is a simple UNIX command line interpreter, created as part of a project for Holberton School. The shell supports various features and commands, allowing users to interact with their system through a command-line interface.

## General Information:

    Who designed and implemented the original Unix operating system?
        Dennis Ritchie and Ken Thompson.

    Who wrote the first version of the UNIX shell?
        Ken Thompson.

    Who invented the B programming language (the direct predecessor to the C programming language)?
        Ken Thompson.

    Who is Ken Thompson?
        Ken Thompson is a computer scientist and one of the co-creators of the Unix operating system.

    How does a shell work?
        A shell is a command-line interpreter that takes commands from the user and executes them, acting as an interface between the user and the operating system.

    What is a pid and a ppid?
        PID (Process ID) is a unique numerical identifier assigned to each running process. PPID (Parent Process ID) is the PID of the parent process that spawned the current process.

    How to manipulate the environment of the current process?
        The environment of the current process can be manipulated using functions like getenv, setenv, and unsetenv.

    What is the difference between a function and a system call?
        A function is a routine that performs a specific task, whereas a system call is a request for a service performed by the kernel.

    How to create processes?
        Processes can be created using the fork system call.

    What are the three prototypes of main?
        The three prototypes of main are int main(void), int main(int argc, char *argv[]), and int main(int argc, char *argv[], char *envp[]).

    How does the shell use the PATH to find the programs?
        The shell searches directories listed in the PATH environment variable to locate executable programs.

    How to execute another program with the execve system call?
        The execve system call replaces the current process image with a new one.

    How to suspend the execution of a process until one of its children terminates?
        The wait system call is used to suspend the execution of a process until one of its child processes exits.

    What is EOF / "end-of-file"?
        EOF is a signal that indicates the end of a file or stream.

## Requirements:

    Allowed editors: vi, vim, emacs.
    Compilation: gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh
    Code style: Follow the Betty style. It will be checked using betty-style.pl and betty-doc.pl.
    Memory leaks: Your shell should not have any memory leaks.
    Functions per file: No more than 5 functions per file.
    Header files: All header files should be include guarded.
    System calls: Use system calls only when necessary.

## Output:

Unless specified otherwise, your program must have the exact same output as sh (/bin/sh) as well as the exact same error output. The only difference is when you print an error, the name of the program must be equivalent to your argv[0].

### Example of error with sh:
$ echo "qwerty" | /bin/sh
/bin/sh: 1: qwerty: not found
$ echo "qwerty" | /bin/../bin/sh
/bin/../bin/sh: 1: qwerty: not found

### Same error with your program hsh:
$ echo "qwerty" | ./hsh
./hsh: 1: qwerty: not found
$ echo "qwerty" | ./././hsh
./././hsh: 1: qwerty: not found

## List of allowed functions and system calls:

    All functions from string.h.
    access (man 2 access)
    chdir (man 2 chdir)
    close (man 2 close)
    closedir (man 3 closedir)
    execve (man 2 execve)
    exit (man 3 exit)
    _exit (man 2 _exit)
    fflush (man 3 fflush)
    fork (man 2 fork)
    free (man 3 free)
    getcwd (man 3 getcwd)
    getline (man 3 getline)
    getpid (man 2 getpid)
    isatty (man 3 isatty)
    kill (man 2 kill)
    malloc (man 3 malloc)
    open (man 2 open)
    opendir (man 3 opendir)
    perror (man 3 perror)
    printf (man 3 printf)
    fprintf (man 3 fprintf)
    vfprintf (man 3 vfprintf)
    sprintf (man 3 sprintf)
    putchar (man 3 putchar)
    read (man 2 read)
    readdir (man 3 readdir)
    signal (man 2 signal)
    stat (__xstat) (man 2 stat)
    lstat (__lxstat) (man 2 lstat)
    fstat (__fxstat) (man 2 fstat)
    strtok (man 3 strtok)
    wait (man 2 wait)
    waitpid (man 2 waitpid)
    wait3 (man 2 wait3)
    wait4 (man 2 wait4)
    write (man 2 write)

## Compilation

Your shell will be compiled this way:
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh

## Testing
Your shell should work in both interactive and non-interactive modes.
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


## Example basic simple shell:

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT 1024

void execute_command(char *command);

int main(void) {
    char input[MAX_INPUT];

    while (1)
{
        printf("$ ");

        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }

        input[strcspn(input, "\n")] = '\0';

        execute_command(input);
    }

    return EXIT_SUCCESS;
}

void execute_command(char *command) {
    pid_t pid, wpid;
    int status;

    pid = fork();

    if (pid == 0) {
        if (execlp(command, command, NULL) == -1) {
            perror("shell");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        perror("shell");
    } else 
{
        do
{
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
}

## Authors

    Yahsai Santana
    Eunielis Serrano
