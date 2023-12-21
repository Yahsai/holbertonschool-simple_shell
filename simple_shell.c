#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>

extern char **environ;

#define MAX_COMMAND_LENGTH 100
#define MAX_ARGS 10

/**
 * prompt - Handle SIGINT signal for a graceful prompt exit
 * @signo: The signal number
 *
 * Return: Nothing
 */
void prompt(int signo)
{
    (void)signo;
    printf("\n$ ");
    fflush(stdout);
}

/**
 * is_builtin_command - Check if a command is a built-in command
 * @command: The command to check
 *
 * Return: 1 if the command is built-in, 0 otherwise
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
 * print_environment - Print the environment variables
 * @env: Array of strings containing environment variables
 *
 * Return: Nothing
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
 * find_command - Find the full path of the command using PATH
 * @command: The command to find
 * @path: The PATH variable
 * @full_path: Buffer to store the full path
 *
 * Return: 1 if the command is found, 0 otherwise
 */
int find_command(char *command, char *path, char *full_path)
{
    char *token;
    char *path_copy;
    char temp_path[MAX_COMMAND_LENGTH];

    path_copy = strdup(path);
    if (path_copy == NULL)
    {
        perror("strdup");
        return 0;
    }

    token = strtok(path_copy, ":");
    while (token != NULL)
    {
        snprintf(temp_path, sizeof(temp_path), "%s/%s", token, command);
        if (access(temp_path, F_OK | X_OK) == 0)
        {
            strcpy(full_path, temp_path);
            free(path_copy);
            return 1;
        }
        token = strtok(NULL, ":");
    }

    free(path_copy);
    return 0;
}

/**
 * exit_shell - Exit the shell
 *
 * Return: Nothing
 */
void exit_shell(void)
{
    printf("Exiting shell\n");
    exit(0);
}

/**
 * env_shell - Print the current environment
 *
 * Return: Nothing
 */
void env_shell(void)
{
    print_environment(environ);
}

/**
 * main - Entry point for the shell
 *
 * Return: Exit status
 */
int main(void)
{
    char input[MAX_COMMAND_LENGTH];
    int status;
    pid_t pid;
    char *tmp_av[MAX_ARGS + 1];
    char *token;
    int ac;
    int has_token;
    int last_exit_status = 0;
    char *path = getenv("PATH");
    char full_path[MAX_COMMAND_LENGTH];

    ssize_t bytesRead;
    char buffer[1024];

    signal(SIGINT, prompt);

    while (1)
    {
        printf("$ ");
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
            exit_shell();
        }
        else if (strcmp(tmp_av[0], "env") == 0)
        {
            env_shell();
        }
        else if (is_builtin_command(tmp_av[0]))
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
        }
        else
        {
            if (find_command(tmp_av[0], path, full_path))
            {
                char **argv = tmp_av;

                int pipefd[2];
                if (pipe(pipefd) == -1)
                {
                    perror("pipe");
                    last_exit_status = 1;
                    continue;
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
                    close(pipefd[0]);

                    if (dup2(pipefd[1], STDOUT_FILENO) == -1)
                    {
                        perror("dup2");
                        last_exit_status = 1;
                        exit(EXIT_FAILURE);
                    }

                    close(pipefd[1]);

                    if (execv(full_path, argv) == -1)
                    {
                        perror("execv");
                        last_exit_status = 127;
                        exit(EXIT_FAILURE);
                    }
                }
                else
                {
                    close(pipefd[1]);

                    while ((bytesRead = read(pipefd[0], buffer, sizeof(buffer))) > 0)
                    {
                        write(STDOUT_FILENO, buffer, bytesRead);
                    }

                    close(pipefd[0]);

                    if (waitpid(pid, &status, 0) == -1)
                    {
                        perror("waitpid");
                        last_exit_status = 1;
                    }
                    else
                    {
                        last_exit_status = WIFEXITED(status) ? WEXITSTATUS(status) : 1;
                    }
                }
            }
            else
            {
                fprintf(stderr, "Command not found: %s\n", tmp_av[0]);
                last_exit_status = 127;
            }
        }
    }

    return last_exit_status;
}

