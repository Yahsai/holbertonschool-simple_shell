#ifndef MAIN_H
#define MAIN_H

/**
 * display_prompt - Display the shell prompt
 */
void display_prompt(void);

/**
 * execute_command - Execute a command in a child process
 * @command: The command to be executed
 */
void execute_command(char *command);

/**
 * is_exit_command - Check if the command is the exit built-in
 * @command: The command to be checked
 * Return: 1 if it's the exit command, 0 otherwise
 */
int is_exit_command(char *command);

/**
 * is_env_command - Check if the command is the env built-in
 * @command: The command to be checked
 * Return: 1 if it's the env command, 0 otherwise
 */
int is_env_command(char *command);

/**
 * execute_env_command - Execute the env built-in command
 */
void execute_env_command(void);

#endif /* MAIN_H */

