#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>

#define MAX_COMMAND_LENGTH 1024

/**
 * prompt - Handle SIGINT signal for a graceful prompt exit
 * @signo: The signal number
 *
 * Return: Nothing
 */
void prompt(int signo);

/**
 * find_command - Find the full path of a command in the given path
 * @command: The command to find
 * @path: The PATH environment variable
 *
 * Return: The full path of the command if found, otherwise NULL
 */
char *find_command(const char *command, const char *path);

/**
 * handle_exit - Perform cleanup and exit the shell
 *
 * Return: Nothing
 */
void handle_exit(void);

/**
 * main - Entry point for the simple shell program
 *
 * Return: Always 0 (success)
 */
int main(void)
{
    char input[MAX_COMMAND_LENGTH];
    ssize_t bytesRead;
    pid_t pid;
    char *full_path;

    signal(SIGINT, prompt);

    while (1)
    {
        write(STDOUT_FILENO, "$ ", 2);

        if ((bytesRead = read(STDIN_FILENO, input, sizeof(input))) == -1)
        {
            perror("read");
            exit(EXIT_FAILURE);
        }

        if (bytesRead == 0)
        {
            break;
        }

        input[bytesRead - 1] = '\0';

        if (strcmp(input, "exit") == 0)
        {
            handle_exit();
            break;
        }

        full_path = find_command(input, getenv("PATH"));

        if (full_path != NULL)
        {
            int status;

            pid = fork();
            if (pid == 0)
            {
                char *args[2];
                args[0] = full_path;
                args[1] = NULL;
                if (execv(full_path, args) == -1)
                {
                    perror("execv");
                    exit(EXIT_FAILURE);
                }
            }
            else if (pid < 0)
            {
                perror("fork");
                exit(EXIT_FAILURE);
            }
            else
            {
                do
                {
                    waitpid(pid, &status, WUNTRACED);
                } while (!WIFEXITED(status) && !WIFSIGNALED(status));

                free(full_path);
            }
        }
        else
        {
            fprintf(stderr, "./hsh: 1: %s: not found\n", input);
        }
    }

    return 0;
}

/**
 * prompt - Handle SIGINT signal for a graceful prompt exit
 * @signo: The signal number
 *
 * Return: Nothing
 */
void prompt(int signo)
{
    (void)signo;
    write(STDOUT_FILENO, "\n$ ", 3);
}

/**
 * find_command - Find the full path of a command in the given path
 * @command: The command to find
 * @path: The PATH environment variable
 *
 * Return: The full path of the command if found, otherwise NULL
 */
char *find_command(const char *command, const char *path)
{
    char *token;
    char *path_copy = strdup(path);

    token = strtok(path_copy, ":");
    while (token != NULL)
    {
        size_t full_path_len = strlen(token) + strlen(command) + 2;
        char *full_path = malloc(full_path_len);
        if (full_path == NULL)
        {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
        snprintf(full_path, full_path_len, "%s/%s", token, command);

        if (access(full_path, X_OK) == 0)
        {
            free(path_copy);
            return full_path;
        }

        free(full_path);
        token = strtok(NULL, ":");
    }

    free(path_copy);
    return NULL;
}

/**
 * handle_exit - Perform cleanup and exit the shell
 *
 * Return: Nothing
 */
void handle_exit(void)
{
    /* Perform any necessary cleanup before exiting */
    printf("Exiting the shell. Goodbye!\n");
    exit(EXIT_SUCCESS);
}

