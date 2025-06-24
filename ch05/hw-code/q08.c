/*
 * 8. Write a program that creates two children, and connects the stan-
 *    dard output of one to the standard input of the other, using the
 *    pipe() system call.
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
  int pipedes[2];

  if (pipe(pipedes) == -1) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  pid_t child1_pid = fork();

  if (child1_pid < 0) {
    perror("child 1 failed");
    exit(EXIT_FAILURE);
  }
  if (child1_pid == 0) {
    close(pipedes[0]);

    if (dup2(pipedes[1], STDOUT_FILENO) == -1) {
      perror("dup2 (child 1) failed");
    }

    close(pipedes[1]);

    printf("This is the first line\n");
    printf("This is the second line\n");

    exit(EXIT_SUCCESS);
  }

  pid_t child2_pid = fork();

  if (child2_pid < 0) {
    perror("child 2 failed");
    exit(EXIT_FAILURE);
  }
  if (child2_pid == 0) {
    close(pipedes[1]);

    if (dup2(pipedes[0], STDIN_FILENO) == -1) {
      perror("dup2 (child 2) failed");
    }

    close(pipedes[0]);

    execlp("wc", "wc", "-l", NULL);

    perror("execlp failed");
    exit(EXIT_FAILURE);
  }

  close(pipedes[0]);
  close(pipedes[1]);

  waitpid(child1_pid, NULL, 0);
  waitpid(child2_pid, NULL, 0);

  return EXIT_SUCCESS;
}
