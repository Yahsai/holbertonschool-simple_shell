#include "my_shell.h"

/**
 * get_full_path - Get the full path of a command
 * @command_name: The name of the command
 * Return: A dynamically allocated string containing the full path
 *         NULL if the command is not found
 */
char *get_full_path(char *command_name)
{
    char *base_path = "/usr/bin/";
    char *full_path;

    if (access(command_name, X_OK) == 0)
    {
        full_path = strdup(command_name);
        if (full_path == NULL)
        {
            perror("malloc");
            return (NULL);
        }
        return (full_path);
    }

    full_path = malloc(strlen(base_path) + strlen(command_name) + 1);
    if (full_path == NULL)
    {
        perror("malloc");
        return (NULL);
    }

    sprintf(full_path, "%s%s", base_path, command_name);
    if (access(full_path, X_OK) != 0)
    {
        perror("Command not found");
        free(full_path);
        return (NULL);
    }

    return (full_path);
}

/**
 * Tokenize_execute - Tokenize a command string and execute it
 * @command_string: The command string to tokenize
 * @env: The environment variables
 */
void Tokenize_execute(char *command_string, char **env)
{
    char *token = NULL;
    char **tokens = NULL;
    int index = 0;
    char *full_path;

    token = strtok(command_string, " \n"); /* Tokenize the command string */
    if (token == NULL)
        return;

    while (token != NULL)
    {
        tokens = realloc(tokens, sizeof(char *) * (index + 1));
        if (tokens == NULL)
        {
            perror("realloc");
            return;
        }

        tokens[index] = strdup(token); /* Store token in tokens array */
        index++;                       /* Increment the token count */
        token = strtok(NULL, " \n");   /* Get next token from command string */
    }

    tokens = realloc(tokens, sizeof(char *) * (index + 1));
    if (tokens == NULL)
    {
        perror("realloc");
        return;
    }

    tokens[index] = NULL;                 /* Set next element in the array to NULL */
    full_path = get_full_path(tokens[0]); /* Get the full path of the command */
    if (full_path == NULL)
    {
        free_tokens(tokens, index); /* Fix the function name */
        return;
    }

    free(tokens[0]);
    tokens[0] = full_path;
    create_child_process(tokens, env);
    free_tokens(tokens, index); /* Fix the function name */
}

/**
 * create_child_process - Create a child process and execute the command
 * @tokens: The tokenized command and arguments
 * @env: The environment variables
 */
void create_child_process(char **tokens, char **env)
{
    pid_t pid = fork(); /* Create a child process */
    int status;

    if (pid == -1)
    {
        perror("fork"); /* Print error message if fork fails */
        return;
    }

    if (pid == 0)
    {
        execve(tokens[0], tokens, env); /* Execute the command using execve */
        perror("error ");               /* Print an error message if execve fails */
        exit(EXIT_FAILURE);             /* Exit child process with failure status */
    }
    else
    {
        /* Wait specifically for the child process to complete */
        waitpid(pid, &status, 0);
    }
}

