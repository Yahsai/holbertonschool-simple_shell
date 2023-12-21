#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>

#define MAX_COMMAND_LENGTH 1024

void prompt(int signo);
char *find_command(const char *command, const char *path);
void handle_exit(void);

#endif /* MAIN_H */

