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
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define ITERATIONS 100000
#define CONTEXT_SWITCHES_PER_ITERATION 2

long long elapsed_nanoseconds(struct timespec start, struct timespec end) {
  return (end.tv_sec - start.tv_sec) * 1000000000LL +
         (end.tv_nsec - start.tv_nsec);
}

int main(void) {
  struct timespec startTime, endTime;
  cpu_set_t set;
  CPU_ZERO(&set);
  CPU_SET(0, &set);
  int pipedes1[2], pipedes2[2];

  if (pipe(pipedes1) == -1) // Create first pipe
    err(EXIT_FAILURE, "pipedes1");
  if (pipe(pipedes2) == -1) // Create second pipe
    err(EXIT_FAILURE, "pipedes2");

  if (sched_setaffinity(getpid(), sizeof(set), &set) == -1)
    err(EXIT_FAILURE, "sched_setaffinity");

  switch (fork()) { // Create 2 processes
  case -1:
    err(EXIT_FAILURE, "fork");

  case 0:
    if (close(pipedes1[0]) == -1) // Close read end of first pipe
      err(EXIT_FAILURE, "close");
    if (close(pipedes2[1]) == -1) // Close write end of second pipe
      err(EXIT_FAILURE, "close");

    char msg1 = 'c';
    char buf1;

    for (int i = 0; i < ITERATIONS; i++) {
      write(pipedes1[1], &msg1, 1); // Write to first pipe
      read(pipedes2[0], &buf1, 1);  // Read from second pipe
    }

    exit(EXIT_SUCCESS);

  default:
    if (close(pipedes1[1]) == -1) // Close write end of first pipe
      err(EXIT_FAILURE, "close");
    if (close(pipedes2[0]) == -1) // Close read end of second pipe
      err(EXIT_FAILURE, "close");

    char msg2 = 'c';
    char buf2;

    printf("Starting timer...\n");
    clock_gettime(CLOCK_MONOTONIC, &startTime);
    for (int i = 0; i < ITERATIONS; i++) {
      read(pipedes1[0], &buf2, 1);  // Read from first pipe
      write(pipedes2[1], &msg2, 1); // Write to second pipe
    }
    clock_gettime(CLOCK_MONOTONIC, &endTime);

    // Calculate and print results
    long long total_elapsed_ns = elapsed_nanoseconds(startTime, endTime);
    long long average_ns =
        total_elapsed_ns / (ITERATIONS * CONTEXT_SWITCHES_PER_ITERATION);

    printf("Performed %d iterations.\n", ITERATIONS);
    printf("Total time: %lld ns\n", total_elapsed_ns);
    printf("Average cost of context-switch: %lld ns\n", average_ns);

    wait(NULL);
    exit(EXIT_SUCCESS);
  }

  return EXIT_SUCCESS;
}
