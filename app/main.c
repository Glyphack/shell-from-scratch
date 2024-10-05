#include <dirent.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

int main() {
  const char *builtin_commands[] = {
      "break", "continue", "eval",    "break",  "continue", "eval",
      "exec",  "exit",     "export",  "newgrp", "readonly", "return",
      "set",   "shift",    "times",   "trap",   "typeset",  "type",
      "unset", "Regular",  "alias",   "bg",     "cd",       "echo",
      "fc",    "fg",       "getopts", "jobs",   "kill",     "let",
      "print", "pwd",      "read",    "ulimit", "umask",    "unalias",
      "wait",  "whence",
  };
  while (1) {
    char *path_env = getenv("PATH");
    printf("$ ");
    fflush(stdout);

    int commandLen = 100;
    char input[commandLen];
    fgets(input, commandLen, stdin);

    for (int i = 0; i < commandLen; i++) {
      char ch = input[i];
      if (ch == '\n') {
        input[i] = 0;
      }
    }

    char *delim = " ";
    char *commandName = strtok(input, delim);

    if (commandName == NULL) {
      return 1;
    }

    if (strcmp(commandName, "exit") == 0) {
      char *exitValue = strtok(NULL, " ");
      if (exitValue == NULL) {
        return 1;
      }
      return atoi(exitValue);
    } else if (strcmp(commandName, "echo") == 0) {
      int first = 1;
      char *token = strtok(NULL, delim);
      while (token != NULL) {
        if (first == 0) {
          printf(" ");
        }
        printf("%s", token);
        token = strtok(NULL, delim);
        first = 0;
      }
      printf("\n");
    } else if (strcmp(commandName, "type") == 0) {

      char *first_arg = strtok(NULL, delim);
      if (first_arg == NULL) {
        return 1;
      }

      int found = 0;
      for (int i = 0; i < 38; i++) {
        if (strcmp(first_arg, builtin_commands[i]) == 0) {
          found = 1;
          printf("%s is a shell builtin\n", first_arg);
          break;
        }
      }
      if (found == 1) {
        continue;
      }

      if (path_env != NULL) {
        char *curr_path = strtok(path_env, ":");
        while (curr_path != NULL) {
          DIR *dir;
          struct dirent *dp;
          if ((dir = opendir(curr_path)) == NULL) {
            printf("reading %s ", curr_path);
            fflush(stdout);
            exit(1);
          }

          while ((dp = readdir(dir)) != NULL) {

            if (strcmp(first_arg, dp->d_name) == 0) {
              printf("%s is %s/%s\n", first_arg, curr_path, dp->d_name);
              found = 1;
            }
          }
          curr_path = strtok(NULL, ":");
        }
      }

      if (found == 1) {
        continue;
      }

      printf("%s: not found\n", first_arg);
    } else {
      printf("%s: command not found\n", input);
    }
    fflush(stdout);
  }
  return 0;
}
