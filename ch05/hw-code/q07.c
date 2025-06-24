/*
 * 7. Write a program that creates a child process, and then in the child
 *    closes standard output (STDOUT FILENO). What happens if the child
 *    calls printf() to print some output after closing the descriptor?
 */

/*
 * When the child calls printf() after closing stanard output, the function
 * attempts to write to the closed stream, resulting in a failure.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
  pid_t fork_pid = fork();

  if (fork_pid < 0) {
    perror("fork failed");
  } else if (fork_pid == 0) {
    printf("CHILD: closing standard output...\n");
    fclose(stdout);
    printf("CHILD: attempting to print...\n");
  } else {
    wait(NULL);
  }

  return EXIT_SUCCESS;
}
