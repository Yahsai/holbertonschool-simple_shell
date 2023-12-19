#include "shell.h"

/**
 * handle_builtin_command - handles execution of built-in commands
 * @command: array of strings containing the command and its arguments
 * @last_exit_status: the status of the last command executed
 * @env: array of strings containing environment variables
 */

void handle_builtin_command(char **command, int *last_exit_status, char **env)
{
	if (strcmp(command[0], "cd") == 0)

	else if (strcmp(command[0], "env") == 0)
	{
		print_environment(env);
	}
}
