#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Declare strdup function */
char *strdup(const char *s);

#include <unistd.h>
#include <sys/wait.h>

#define MAX_INPUT_SIZE 1024
#define MAX_TOKENS 100

void free_tokens(char **tokens) {
    for (int i = 0; tokens[i] != NULL; ++i) {
        free(tokens[i]);
    }
    free(tokens);
}

void execute_command(char *command) {
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        /* Child process */
        execlp(command, command, NULL);

        /* If execlp fails */
        perror("exec");
        exit(EXIT_FAILURE);
    } else {
        /* Parent process */
        wait(NULL);
    }
}

char **tokenize_input(char *input) {
    char **tokens = (char **)malloc(MAX_TOKENS * sizeof(char *));
    if (!tokens) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    char *token = strtok(input, " ");
    int token_count = 0;

    while (token != NULL && token_count < MAX_TOKENS - 1) {
        tokens[token_count++] = strdup(token);
        token = strtok(NULL, " ");
    }

    tokens[token_count] = NULL;

    return tokens;
}

int main() {
    char input[MAX_INPUT_SIZE];

    while (1) {
        /* Display shell prompt */
        printf("($) ");
        fflush(stdout);

        /* Read user input */
        if (fgets(input, sizeof(input), stdin) == NULL) {
            /* Handle Ctrl+D or other errors */
            break;
        }

        /* Remove newline character */
        input[strcspn(input, "\n")] = '\0';

        /* Check for the "exit" command */
        if (strcmp(input, "exit") == 0) {
            break;
        }

        /* Tokenize the input */
        char **tokens = tokenize_input(input);

        if (tokens[0] != NULL) {
            /* Execute the command */
            execute_command(tokens[0]);

            /* Free the allocated memory for tokens */
            free_tokens(tokens);
        }
    }

    printf("\n"); /* Add a newline for better shell appearance */
    return 0;
}

