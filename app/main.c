#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main() {
  while (1) {
    printf("$ ");
    fflush(stdout);

    int commandLen = 100;
    char input[commandLen];
    fgets(input, commandLen, stdin);

    for (int i=0;i<commandLen;i++) {
      char ch = input[i];
      if (ch == '\n') {
        input[i] = 0;
      }
    }

    char* delim = " ";
    char* commandName = strtok(input,delim );

    if (commandName == NULL) {
      return 1;
    }

    if (strcmp(commandName, "exit") == 0) {
      char * exitValue = strtok(NULL, " ");
      if (exitValue == NULL) {
        return 1;
      }
      return atoi(exitValue);
    } else if (strcmp(commandName, "echo") == 0) {
      int first = 1;
      char* token = strtok(NULL, delim);
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
      char* builtin_commands[37] = {
        "break",
        "continue",
        "eval",
        "break",
        "continue",
        "eval",
        "exec",
        "exit",
        "export",
        "newgrp",
        "readonly",
        "return",
        "set",
        "shift",
        "times",
        "trap",
        "typeset",
        "unset",
        "Regular",
        "alias",
        "bg",
        "cd",
        "echo",
        "fc",
        "fg",
        "getopts",
        "jobs",
        "kill",
        "let",
        "print",
        "pwd",
        "read",
        "ulimit",
        "umask",
        "unalias",
        "wait",
        "whence",
      };

      char* first_arg = strtok(NULL, delim);
      if (first_arg == NULL) {
        return 1;
      }

      int found = 0;
      for (int i=0; i<37;i++) {
        if (strcmp(first_arg, builtin_commands[i]) == 0)
        {
          found = 1;
        }
      }

      if (found == 1) {
        printf("found\n");
      } else {
        printf("not found\n");
      }
    } else {
    printf("%s: command not found\n", input);
    }
    fflush(stdout);
  }
  return 0;
}
