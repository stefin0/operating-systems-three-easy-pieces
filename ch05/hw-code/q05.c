/*
 * 5. Now write a program that uses wait() to wait for the child process
 *    to finish in the parent. What does wait() return? What happens if
 *    you use wait() in the child?
 */

/*
 * When a child process dies, wait() returns the child's process id. For
 * errors, returns -1.
 * If wait() is called in a child process that has no children themselves, then
 * wait will fail, returning -1.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
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

    pid_t terminated_child_pid = wait(NULL);

    printf("PARENT: My child (PID: %d) has finished.\n", terminated_child_pid);
    printf("PARENT: Parent process is now finishing.\n");
  }

  return EXIT_SUCCESS;
}
