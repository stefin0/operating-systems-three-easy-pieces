/*
 * In this homework, you’ll measure the costs of a context switch. The lmbench
 * benchmark does so by running two processes on a single CPU, and set-
 * ting up two UNIX pipes between them; a pipe is just one of many ways
 * processes in a UNIX system can communicate with one another. The first
 * process then issues a write to the first pipe, and waits for a read on the
 * second; upon seeing the first process waiting for something to read from
 * the second pipe, the OS puts the first process in the blocked state, and
 * switches to the other process, which reads from the first pipe and then
 * writes to the second. When the second process tries to read from the first
 * pipe again, it blocks, and thus the back-and-forth cycle of communication
 * continues. By measuring the cost of communicating like this repeatedly,
 * lmbench can make a good estimate of the cost of a context switch. You
 * can try to re-create something similar here, using pipes, or perhaps some
 * other communication mechanism such as UNIX sockets.
 * One difficulty in measuring context-switch cost arises in systems with
 * more than one CPU; what you need to do on such a system is ensure that
 * your context-switching processes are located on the same processor. For-
 * tunately, most operating systems have calls to bind a process to a partic-
 * ular processor; on Linux, for example, the sched_setaffinity() call
 * is what you’re looking for. By ensuring both processes are on the same
 * processor, you are making sure to measure the cost of the OS stopping
 * one process and restoring another on the same CPU.
 */
#define _GNU_SOURCE
#include <err.h>
#include <sched.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
  cpu_set_t set;
  int pipedes1[2], pipedes2[2];

  // Create 2 pipes
  if (pipe(pipedes1) == -1)
    err(EXIT_FAILURE, "pipedes1");
  if (pipe(pipedes2) == -1)
    err(EXIT_FAILURE, "pipedes2");

  CPU_ZERO(&set);

  // Create 2 processes
  switch (fork()) {
  case -1:
    err(EXIT_FAILURE, "fork");

  case 0:
    CPU_SET(0, &set);

    if (sched_setaffinity(getpid(), sizeof(set), &set) == -1)
      err(EXIT_FAILURE, "sched_setaffinity");

    // Close reading end of first pipe and writing end of second pipe
    if (close(pipedes1[0]) == -1)
      err(EXIT_FAILURE, "close");
    if (close(pipedes2[1]) == -1)
      err(EXIT_FAILURE, "close");

    // Write to first pipe
    char msg = 'c';
    if (write(pipedes1[1], &msg, 1) == -1)
      err(EXIT_FAILURE, "write");

    // Read from second pipe
    char buf1;
    if (read(pipedes2[0], &buf1, 1) == -1)
      err(EXIT_FAILURE, "read");

    exit(EXIT_SUCCESS);

  default:
    CPU_SET(0, &set);

    if (sched_setaffinity(getpid(), sizeof(set), &set) == -1)
      err(EXIT_FAILURE, "sched_setaffinity");

    // Close writing end of first pipe and reading end of second pipe
    if (close(pipedes1[1]) == -1)
      err(EXIT_FAILURE, "close");
    if (close(pipedes2[0]) == -1)
      err(EXIT_FAILURE, "close");

    // Read from first pipe
    char buf2;
    if (read(pipedes1[0], &buf2, 1) == -1)
      err(EXIT_FAILURE, "read");

    // Write to second pipe

    wait(NULL);
    exit(EXIT_SUCCESS);
  }

  return EXIT_SUCCESS;
}
