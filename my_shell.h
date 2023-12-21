#ifndef MY_SHELL_H
#define MY_SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

char *get_full_path(char *command_name);
void free_tokens(char **tokens, int count);
void Tokenize_execute(char *command_string, char **env);
void create_child_process(char **tokens, char **env);

#endif
