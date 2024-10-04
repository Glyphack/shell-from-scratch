#include <stdio.h>

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

    printf("%s: command not found\n", input);
    fflush(stdout);
  }
  return 0;
}
