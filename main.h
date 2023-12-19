#ifndef SHELL_HEADER_H
#define SHELL_HEADER_H

/**
 * MAX_COMMAND_LENGTH - Maximum length of a shell command
 */
#define MAX_COMMAND_LENGTH 100

/**
 * MAX_ARGS - Maximum number of arguments for a shell command
 */
#define MAX_ARGS 10

/**
 * is_builtin_command - determines whether given command is built-in or not
 * @command: the given command
 * Return: 0 if not built-in, 1 if built-in
 */
int is_builtin_command(char *command);

/**
 * print_environment - prints the environment variables
 * @env: array of strings containing environment variables
 * Return: nothing
 */
void print_environment(char **env);

/**
 * prompt - returns to prompt. Used to ignore SIGINT
 * @signo: the signal number
 * Return: nothing
 */
void prompt(int signo);

/**
 * execute_command - executes a non-built-in shell command
 * @av: array of strings containing command and arguments
 * @env: array of strings containing environment variables
 * @last_exit_status: pointer to the last exit status
 * Return: nothing
 */
void execute_command(char **av, char **env, int *last_exit_status);

/**
 * handle_builtin_command - handles the execution of built-in shell commands
 * @command: array of strings containing command and arguments
 * @last_exit_status: pointer to the last exit status
 * @env: array of strings containing environment variables
 * Return: nothing
 */
void handle_builtin_command(char **command, int *last_exit_status, char **env);

#endif /* SHELL_HEADER_H */

