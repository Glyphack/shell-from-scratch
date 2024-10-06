#include <dirent.h>
#include <errno.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

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

  char *path_env = getenv("PATH");
  int path_count = 0;
  if (path_env != NULL) {
    path_count = 1;
  }
  for (int i = 0; path_env[i] != 0; i++) {
    if (path_env[i] == ':')
      path_count++;
  }

  fflush(stdout);

  char *paths[path_count];

  char *delim_env = ":";
  char *curr_path = strtok(path_env, delim_env);
  for (int i = 0; i < path_count; i++) {
    fflush(stdout);
    paths[i] = curr_path;
    curr_path = strtok(NULL, delim_env);
  }

  while (1) {
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
    } else if (strcmp(commandName, "pwd") == 0) {
      int size = pathconf(".", _PC_PATH_MAX);
      char pwd[size];
      char *ptr = getcwd(pwd, (size_t)size);
      if (ptr == NULL)
        exit(1);

      printf("%s\n", ptr);
    } else if (strcmp(commandName, "cd") == 0) {
      char *location = strtok(NULL, " ");
      if (strcmp(location, "~") == 0) {
        location = getenv("HOME");
      }
      if (location == NULL)
        exit(1);
      int status = chdir(location);
      if (status != 0) {
        printf("cd: %s: No such file or directory\n", location);
      }
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

      if (path_count > 0) {
        DIR *dir;
        struct dirent *dp;
        for (int i = 0; i < path_count; i++) {
          curr_path = paths[i];
          if ((dir = opendir(curr_path)) == NULL) {
            continue;
          }

          while ((dp = readdir(dir)) != NULL) {
            if (strcmp(first_arg, dp->d_name) == 0) {
              printf("%s is %s/%s\n", first_arg, curr_path, dp->d_name);
              found = 1;
            }
          }
          closedir(dir);
          if (found == 1)
            break;
        }
      }

      if (found == 1) {
        continue;
      }

      printf("%s: not found\n", first_arg);
    } else {
      char *foundCommand = NULL;
      if (path_count > 0) {
        DIR *dir;
        struct dirent *dp;
        for (int i = 0; i < path_count; i++) {
          printf("path: %s\n", curr_path);
          curr_path = paths[i];
          if ((dir = opendir(curr_path)) == NULL) {
            continue;
          }

          while ((dp = readdir(dir)) != NULL) {
            if (strcmp(commandName, dp->d_name) == 0) {
              char newCommand[commandLen];
              strcpy(newCommand, curr_path);
              strcat(newCommand, "/");
              strcat(newCommand, dp->d_name);
              foundCommand = malloc(strlen(newCommand) + 1);
              if (foundCommand != NULL) {
                strcpy(foundCommand, newCommand);
              }
            }
          }
          closedir(dir);
        }
      }
      if (foundCommand != NULL) {
        char *token = strtok(NULL, delim);
        while (token != NULL) {
          strcat(foundCommand, " ");
          strcat(foundCommand, token);
          token = strtok(NULL, delim);
        }
        int status = system(foundCommand);
        continue;
      }
      printf("%s: command not found\n", input);
    }
    fflush(stdout);
  }
  return 0;
}
