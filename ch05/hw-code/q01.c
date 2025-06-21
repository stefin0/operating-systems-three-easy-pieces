/*
1. Write a program that calls fork(). Before calling fork(), have the
main process access a variable (e.g., x) and set its value to something
(e.g., 100). What value is the variable in the child process?
What happens to the variable when both the child and parent change
the value of x?
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
  // main process accesses variable x, and set value to 100
  int x = 100;

  // Call fork()
  int rc = fork();
  if (rc == -1) {
    // Fork failed
    fprintf(stderr, "fork failed\n");
    exit(1);
  } else if (rc == 0) {
    // child (new process)
    printf("x: %d (child)\n", x); // x: 100
    x = 11;
    printf("Updated x: %d (child)\n", x); // x: 11
  } else {
    // parent (main)
    printf("x: %d (parent)\n", x); // x: 100
    x = 10;
    printf("Updated x: %d (parent)\n", x); // x: 10
  }

  return 0;
}
