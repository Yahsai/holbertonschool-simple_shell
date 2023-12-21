#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 100

extern char **environ;

char *get_full_path(char *command);
char **tokenize_command(char *command);
void execute_command(char **tokens, char **env);

int main(void)
{
    char input[MAX_COMMAND_LENGTH + 1];
    char **tokens;

    while (1)
    {
        printf("($) ");
        fflush(stdout);

        if (fgets(input, sizeof(input), stdin) == NULL)
            break;

        input[strcspn(input, "\n")] = '\0';

        if (strlen(input) == 0)
            continue;

        tokens = tokenize_command(input);
        if (tokens == NULL)
            continue;

        execute_command(tokens, environ);

        // Free memory allocated for tokens
        for (int i = 0; tokens[i] != NULL; i++)
        {
            free(tokens[i]);
        }
        free(tokens);
    }

    return 0;
}

char *get_full_path(char *command)
{
    char *comd_path = "/bin/";
    char *conpath;

    if (access(command, X_OK) == 0)
    {
        conpath = strdup(command);
        if (conpath == NULL)
        {
            perror("malloc");
            return NULL;
        }
        return conpath;
    }

    conpath = malloc(strlen(comd_path) + strlen(command) + 1);
    if (conpath == NULL)
    {
        perror("malloc");
        return NULL;
    }

    sprintf(conpath, "%s%s", comd_path, command);
    if (access(conpath, X_OK) != 0)
    {
        perror("Command not found");
        free(conpath);
        return NULL;
    }

    return conpath;
}

char **tokenize_command(char *command)
{
    char *tok = NULL;
    char **tokens = NULL;
    int index = 0;

    tok = strtok(command, " \n");
    if (tok == NULL)
        return NULL;

    while (tok != NULL)
    {
        tokens = realloc(tokens, sizeof(char *) * (index + 1));
        if (tokens == NULL)
        {
            perror("realloc");
            return NULL;
        }
        tokens[index] = strdup(tok);
        index++;
        tok = strtok(NULL, " \n");
    }

    tokens = realloc(tokens, sizeof(char *) * (index + 1));
    if (tokens == NULL)
    {
        perror("realloc");
        return NULL;
    }

    tokens[index] = NULL;
    return tokens;
}

void execute_command(char **tokens, char **env)
{
    char *full_path = get_full_path(tokens[0]);

    if (full_path == NULL)
        return;

    free(tokens[0]);
    tokens[0] = full_path;

    pid_t pid = fork();

    if (pid == -1)
    {
        perror("fork");
        return;
    }

    if (pid == 0)
    {
        execve(tokens[0], tokens, env);
        perror("execve");
        exit(EXIT_FAILURE);
    }
    else
    {
        int status;
        waitpid(pid, &status, 0);
    }
}

