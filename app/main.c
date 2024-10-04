#include <stdio.h>

int main() {
  printf("$ ");
  fflush(stdout);

  char input[100];
  fgets(input, 100, stdin);
  return 0;
}
