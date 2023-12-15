#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_INPUT_SIZE 1024

/**
 * display_prompt - Display the shell prompt
 */
void display_prompt(void)
{
    printf("simple_shell$ ");
}

/**
 * execute_command - Execute a command in a child process
 * @command: The command to be executed
 */
void execute_command(char *command)
{
    pid_t pid = fork();

    if (pid == -1)
    {
        perror("fork");
    }
    else if (pid == 0) /* Child process */
    {
        char *args[MAX_INPUT_SIZE];
        char *token;
        int i = 0;

        /* Tokenize the command */
        token = strtok(command, " \t\n");
        while (token != NULL)
        {
            args[i++] = token;
            token = strtok(NULL, " \t\n");
        }
        args[i] = NULL;

        /* Execute the command using PATH */
        execvp(args[0], args);

        /* If execvp fails */
        perror("execvp");

        /* Exit the child process if the command doesn't exist */
        exit(EXIT_FAILURE);
    }
    else /* Parent process */
    {
        /* Wait for the child to complete */
        waitpid(pid, NULL, 0);
    }
}

/**
 * is_exit_command - Check if the command is the exit built-in
 * @command: The command to be checked
 * Return: 1 if it's the exit command, 0 otherwise
 */
int is_exit_command(char *command)
{
    return strcmp(command, "exit") == 0;
}

/**
 * is_env_command - Check if the command is the env built-in
 * @command: The command to be checked
 * Return: 1 if it's the env command, 0 otherwise
 */
int is_env_command(char *command)
{
    return strcmp(command, "env") == 0;
}

/**
 * execute_env_command - Execute the env built-in command
 */
void execute_env_command(void)
{
    extern char **environ;

    /* Print the current environment */
    char **env;
    for (env = environ; *env != NULL; env++)
    {
        printf("%s\n", *env);
    }
}

/**
 * main - Entry point of the simple shell
 * Return: Always 0
 */
int main(void)
{
    char input[MAX_INPUT_SIZE];

    while (1)
    {
        /* Display prompt */
        display_prompt();

        /* Read user input */
        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            /* Handle end of file (Ctrl+D) */
            printf("\n");
            break;
        }

        /* Remove newline character */
        input[strcspn(input, "\n")] = '\0';

        /* Check for built-in commands */
        if (is_exit_command(input))
        {
            printf("Exiting the shell...\n");
            break;
        }
        else if (is_env_command(input))
        {
            /* Execute the env command */
            execute_env_command();
        }
        else
        {
            /* Execute the command in a child process */
            execute_command(input);
        }
    }

    return 0;
}

