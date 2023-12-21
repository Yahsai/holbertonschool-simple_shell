#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_INPUT_SIZE 1024

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
        // Display shell prompt
        printf("($) ");
        fflush(stdout);

        // Read user input
        if (fgets(input, sizeof(input), stdin) == NULL) {
            // Handle Ctrl+D or other errors
            break;
        }

        // Remove newline character
        input[strcspn(input, "\n")] = '\0';

        // Check for the "exit" command
        if (strcmp(input, "exit") == 0) {
            break;
        }

        // Execute the command
        execute_command(input);
    }

    printf("\n");  // Add a newline for better shell appearance
    return 0;
}

