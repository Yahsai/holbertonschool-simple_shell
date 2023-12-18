#include "shell.h"

/**
 * execute_external_command - executes external commands.
 * @av: array of character pointers representing command arguments.
 * Return: exit status of the last executed command.
 */
int execute_external_command(char **av)
{
	pid_t pid;
	int status;
	int last_exit_status = 0;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		last_exit_status = 1;
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		/* Child process */
		if (execvp(av[0], av) == -1)
		{
			perror("execvp");
			last_exit_status = 127; /* Indicates command not found */
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		/* Parent process */
		wait(&status);

		/* Check if the process terminated normally or with an error */
		if (WIFEXITED(status))
		{
			/* Get the exit status of the child process */
			last_exit_status = WEXITSTATUS(status);
		}
		else
		{
			last_exit_status = 1; /* Status 1 indicates an error in the child process */
		}
	}

	return last_exit_status;
}

