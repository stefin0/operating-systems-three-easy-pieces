/*
3. Write another program using fork(). The child process should
print “hello”; the parent process should print “goodbye”. You should
try to ensure that the child process always prints first; can you do
this without calling wait() in the parent?
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
  int pipedes[2];

  // Create a pipe
  if (pipe(pipedes) == -1) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  pid_t pid = fork();

  if (pid < 0) {
    fprintf(stderr, "fork failed\n");
    exit(EXIT_FAILURE);
  } else if (pid == 0) { // Child process
    // Close reading end of pipe
    close(pipedes[0]);

    // Perform the child task
    printf("hello\n");

    // Signal the parent by writing to the pipe
    char msg = 'c'; // msg is irrelevant
    if (write(pipedes[1], &msg, 1) == -1) {
      perror("write");
      exit(EXIT_FAILURE);
    }

    // Close writing end of pipe and exit
    close(pipedes[1]);
    exit(EXIT_SUCCESS);
  } else { // Parent process
    // Close writing end of pipe
    close(pipedes[1]);

    // Wait for the child to signal
    char buf;
    if (read(pipedes[0], &buf, 1) == -1) {
      perror("read");
      exit(EXIT_FAILURE);
    }

    // Now that the child has signaled, proceed to print
    printf("goodbye\n");

    // Close reading end of pipe
    close(pipedes[0]);
  }

  return 0;
}
