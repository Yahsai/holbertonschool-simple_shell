#include "shell.h"

/**
 * main - Entry point of the program
 * @ac: Number of arguments
 * @av: Array of strings containing the arguments
 * @env: Array of strings containing environment variables
 * Return: Exit status
 */
int main(int ac, char **av, char **env)
{
	char input[MAX_COMMAND_LENGTH];
	int status;
	pid_t pid;
	char *tmp_av[MAX_ARGS + 1];
	char *token;
	int has_token;
	int last_exit_status = 0;

	signal(SIGINT, prompt);

	while (1)
	{
		fflush(stdout);

		if (fgets(input, sizeof(input), stdin) == NULL)
		{
			break;
		}
		input[strcspn(input, "\n")] = '\0';

		if (strlen(input) == 0)
		{
			continue;
		}
		token = strtok(input, " ");
		ac = 0;
		has_token = 0;

		while (token != NULL && ac < MAX_ARGS)
		{
			tmp_av[ac++] = token;
			has_token = 1;
			token = strtok(NULL, " ");
		}

		if (!has_token)
		{
			continue;
		}
		tmp_av[ac] = NULL;

		if (strcmp(tmp_av[0], "exit") == 0)
		{
			last_exit_status = 0;
			break;
		}
		if (is_builtin_command(tmp_av[0]))
		{
			handle_builtin_command(tmp_av, &last_exit_status, env);
		}
		else
		{
			execute_command(tmp_av, av, &last_exit_status);
		}
	}
	return last_exit_status;
}
