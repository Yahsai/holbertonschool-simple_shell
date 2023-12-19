#include "shell.h"

/**
 * execute_command - executes a non-built-in command
 * @av: array of strings containing the command and its arguments
 * @env: array of strings containing environment variables
 * @last_exit_status: the status of the last command executed
 */

void execute_command(char **av, char **env, int *last_exit_status)
{
	pid_t pid;
	int status;

	int i;

	for (i = 0; i < ac; i++)
	{
		av[i] = tmp_av[i];
	}
	av[ac] = NULL;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		*last_exit_status = 1;
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		if (execvp(av[0], av) == -1)
		{
			perror("execvp");
			*last_exit_status = 127;
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		wait(&status);

		if (WIFEXITED(status))
		{
			*last_exit_status = WEXITSTATUS(status);
		}
		else
		{
			*last_exit_status = 1;
		}
	}
}
