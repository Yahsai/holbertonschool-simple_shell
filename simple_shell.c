#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>

#define MAX_COMMAND_LENGTH 100
#define MAX_ARGS 10

/**
 * prompt - Handles the SIGINT signal for a graceful prompt exit
 * @signo: The signal number
 *
 * Return: Nothing
 */
void prompt(int signo)
{
    (void)signo;
    printf("($) ");
    fflush(stdout);
}

/**
 * main - Main function of the program
 * @ac: Number of command line arguments
 * @av: Array of command line arguments
 * @env: Array of program environment variables
 *
 * Return: The program exit code
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
        printf("($) ");
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

        pid = fork();
        if (pid < 0)
        {
            perror("fork");
            last_exit_status = 1;
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {
            if (execvp(tmp_av[0], tmp_av) == -1)
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

    return last_exit_status;
}

