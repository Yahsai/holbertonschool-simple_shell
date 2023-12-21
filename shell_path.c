#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_INPUT_SIZE 1024
#define MAX_TOKENS 100

void execute_command(char *command) {
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process
        execlp(command, command, NULL);

        // If execlp fails
        perror("exec");
        exit(EXIT_FAILURE);
    } else {
        // Parent process
        wait(NULL);
    }
}

int main() {
    char input[MAX_INPUT_SIZE];

    while (1) {
        // Display prompt
        printf(":) ");
        fflush(stdout);

        // Read user input
        if (fgets(input, sizeof(input), stdin) == NULL) {
            // Handle Ctrl+D or other errors
            break;
        }

        // Remove newline character
        input[strcspn(input, "\n")] = '\0';

        // Tokenize the input
        char *tokens[MAX_TOKENS];
        char *token = strtok(input, " ");
        int token_count = 0;

        while (token != NULL && token_count < MAX_TOKENS - 1) {
            tokens[token_count++] = token;
            token = strtok(NULL, " ");
        }

        tokens[token_count] = NULL;

        if (token_count > 0) {
            // Check if the command exists in the PATH
            if (access(tokens[0], X_OK) == 0) {
                // Command exists, execute it
                execute_command(tokens[0]);
            } else {
                // Command not found
                fprintf(stderr, "Command not found: %s\n", tokens[0]);
            }
        }
    }

    printf("\n");  // Add a newline for better shell appearance
    return 0;
}

