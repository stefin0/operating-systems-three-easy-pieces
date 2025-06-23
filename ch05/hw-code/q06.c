/*
 * 5. Now write a program that uses wait() to wait for the child process
 *    to finish in the parent. What does wait() return? What happens if
 *    you use wait() in the child?
 * 6. Write a slight modification of the previous program, this time us-
 *    ing waitpid() instead of wait(). When would waitpid() be
 *    useful?
 */

/*
 * waitpid() would be useful for when you want more control of which process
 * to wait for. The parent process can wait for a particular pid, wait for any
 * pid, and more.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
  int status;

  pid_t pid_fork = fork();
  if (pid_fork < 0) {
    perror("fork failed");
    return EXIT_FAILURE;
  } else if (pid_fork == 0) {
    printf("CHILD: I am the child process.\n");
    printf("CHILD: I will now call wait()...\n");

    // The child calls wait(), but it has no children of its own.
    pid_t wait_return = wait(NULL);

    if (wait_return == -1) {
      printf("CHILD: wait() failed as expected.\n");
      perror("CHILD: The error from wait() is");
    }
    printf("CHILD: Child process is now finishing.\n");

  } else {
    printf("PARENT: I am the parent process, waiting for my child (PID: %d) to "
           "finish.\n",
           pid_fork);

    pid_t terminated_child_pid = waitpid(pid_fork, &status, 0);

    printf("PARENT: My child (PID: %d) has finished.\n", terminated_child_pid);
    printf("PARENT: Parent process is now finishing.\n");
  }

  return EXIT_SUCCESS;
}
