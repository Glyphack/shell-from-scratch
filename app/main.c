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

    char* commandName = strtok(input, " ");

    if (commandName == NULL) {
      return 1;
    }

    if (strcmp(commandName, "exit") == 0) {
      char * exitValue = strtok(NULL, " ");
      if (exitValue == NULL) {
        return 1;
      }
      return atoi(exitValue);
    } else {
    printf("%s: command not found\n", input);
    fflush(stdout);
    }
  }
  return 0;
}
