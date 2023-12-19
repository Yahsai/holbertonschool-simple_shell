#include "shell.h"

/**
 * main - the main shell function.
 * @ac: argument count
 * @av: array of character pointers
 * @env: a pointer to an array of character pointers
 * Return: the exit status of the last executed command.
 */
int main(int ac, char **av, char **env)
{
	struct sigaction sa;
	char input[MAX_COMMAND_LENGTH];
	char *tmp_av[MAX_ARGS + 1];
	char *token;
	int has_token;
	int last_exit_status = 0;

	while (1)
	{
		/* Make sure the prompt is displayed before reading input */
		fflush(stdout);
		sa.sa_handler = &prompt;
		sigaction(SIGINT, &sa, NULL);

		/* Check if Ctrl+D (EOF) is encountered */
		if (fgets(input, sizeof(input), stdin) == NULL)
		{
			break;
		}

		/* Remove the newline character from the input */
		input[strcspn(input, "\n")] = '\0';

		/* Skip processing if input is empty */
		if (strlen(input) == 0)
		{
			continue;
		}

		/* Tokenize the input into separate arguments and store them in tmp_av */
		token = strtok(input, " ");
		ac = 0;
		has_token = 0;

		while (token != NULL && ac < MAX_ARGS)
		{
			tmp_av[ac++] = token;
			has_token = 1;
			token = strtok(NULL, " ");
		}

		/* Check if there are no arguments */
		if (!has_token)
		{
			continue;
		}

		tmp_av[ac] = NULL;

		/* Check for the exit command */
		if (strcmp(tmp_av[0], "exit") == 0)
		{
			/* Set the exit status to 0 as it's a normal termination */
			last_exit_status = 0;
			break; /* Exit the while loop and terminate the shell */
		}

		/* Check for built-in commands */
		if (is_builtin_command(tmp_av[0]))
		{
			if (strcmp(tmp_av[0], "cd") == 0)
			{
				if (ac > 1)
				{
					if (chdir(tmp_av[1]) != 0)
					{
						perror("cd");
						/* Set exit status to 1 for errors in built-in commands */
						last_exit_status = 1;
					}
				}
			}
			else if (strcmp(tmp_av[0], "env") == 0)
			{
				print_environment(env);
			}
		}
		else
		{
			/* Copy the arguments from tmp_av to av before executing the command */
			int i;
			for (i = 0; i < ac; i++)
			{
				av[i] = tmp_av[i];
			}
			av[ac] = NULL;

			last_exit_status = execute_external_command(av);
		}
	}
	return (last_exit_status);
}

