#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>

#define MAX_COMMAND_LENGTH 100
#define MAX_ARGS 10

/**
 * prompt - Handles SIGINT signal for a graceful prompt exit
 * @signo: The signal number
 *
 * Return: Nothing
 */
void prompt(int signo)
{
    (void)signo;
    printf("\n($) ");
    fflush(stdout);
}

/**
 * main - Entry point for the simple shell
 * 
 * Return: Exit status
 */
int main(void)
{
    char input[MAX_COMMAND_LENGTH + 1];
    int status;
    pid_t pid;
    char *tmp_av[MAX_ARGS + 1];
    int ac;
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

        has_token = 0;

        for (ac = 0, token = strtok(input, " "); token != NULL && ac < MAX_ARGS; token = strtok(NULL, " "))
        {
            tmp_av[ac++] = token;
            has_token = 1;
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

