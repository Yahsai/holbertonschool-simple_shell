#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>

#define MAX_COMMAND_LENGTH 100
#define MAX_ARGS 10

/* Additional functions if you have them */

/**
 * is_builtin_command - determines whether given command is built-in or not
 * @command: the given command
 * Return: 0 if not built-in, 1 if built-in
 */
int is_builtin_command(char *command)
{
	if (strcmp(command, "exit") == 0 || strcmp(command, "clear") == 0 ||
			strcmp(command, "cd") == 0 || strcmp(command, "env") == 0)
	{
		return 1;
	}
	return 0;
}

/**
 * print_environment - prints the environment variables
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
 * prompt - returns to prompt. Used to ignore SIGINT
 * @signo: the signal number
 * Return: nothing
 */
void prompt(int signo)
{
	(void)signo;  /* Ignore the unused parameter to avoid a warning */
	printf("\n");
	fflush(stdout);
}
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
			if (strcmp(tmp_av[0], "cd") == 0)
			{
				if (ac > 1)
				{
					if (chdir(tmp_av[1]) != 0)
					{
						perror("cd");
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
				last_exit_status = 1;
				exit(EXIT_FAILURE);
			}
			else if (pid == 0)
			{
				if (execvp(av[0], av) == -1)
				{
					perror("execvp");
					last_exit_status = 127;
					exit(EXIT_FAILURE);
				}
			}
			else
			{
				wait(&status);

				if (WIFEXITED(status))
				{
					last_exit_status = WEXITSTATUS(status);
				}
				else
				{
					last_exit_status = 1;
				}
			}
		}
	}

	return last_exit_status;
}
