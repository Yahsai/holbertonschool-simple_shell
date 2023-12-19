#include "shell.h"

/**
 * is_builtin_command - determines whether the given command is built-in or not.
 * @command: the given command
 * Return: 1 if built-in, 0 otherwise
 */
int is_builtin_command(char *command)
{
	if (strcmp(command, "exit") == 0 || strcmp(command, "clear") == 0 ||
	    strcmp(command, "cd") == 0 || strcmp(command, "env") == 0)
	{
		return (1);
	}
	return (0);
}

/**
 * print_environment - prints the environment variables passed to the program on startup.
 * @env: array of strings containing environment variables
 * Return: nothing
 */
void print_environment(char **env)
{
	while (*env != NULL)
	{
		printf("%s\n", *env);
		env++;
	}
}

/**
 * prompt - returns to the prompt, used to ignore SIGINT.
 * @signo: signal number
 * Return: nothing
 */
void prompt(int signo)
{
	(void)signo;
	printf("\n$ ");
	fflush(stdout);
}

