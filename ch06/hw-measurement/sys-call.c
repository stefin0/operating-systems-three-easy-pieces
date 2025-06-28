/*
 * In this homework, you’ll measure the costs of a system call. Measuring the
 * cost of a system call is relatively easy. For example, you could repeatedly
 * call a simple system call (e.g., performing a 0-byte read), and time how
 * long it takes; dividing the time by the number of iterations gives you an
 * estimate of the cost of a system call. One thing you’ll have to take into
 * account is the precision and accuracy of your timer. A typical timer that
 * you can use is gettimeofday(); read the man page for details. What you’ll
 * see there is that gettimeofday() returns the time in microseconds since
 * 1970; however, this does not mean that the timer is precise to the
 * microsecond. Measure back-to-back calls to gettimeofday() to learn something
 * about how precise the timer really is; this will tell you how many
 * iterations of your null system-call test you’ll have to run in order to get
 * a good measurement result. If gettimeofday() is not precise enough for you,
 * you might look into using the rdtsc instruction available on x86 machines.
 */
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define ITERATIONS 10000000

long long elapsed_nanoseconds(struct timespec start, struct timespec end) {
  return (end.tv_sec - start.tv_sec) * 1000000000LL +
         (end.tv_nsec - start.tv_nsec);
}

int main(void) {
  struct timespec startTime, endTime;
  const char *filename = "testfile.txt";
  char buffer[1];

  // Measure Timer Resolution
  clock_gettime(CLOCK_MONOTONIC, &startTime);
  clock_gettime(CLOCK_MONOTONIC, &endTime);
  long long timer_resolution = elapsed_nanoseconds(startTime, endTime);
  printf("Timer resolution: %lld ns\n", timer_resolution);

  // Create a dummy file
  int fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
  if (fd == -1) {
    perror("open for writing");
    return EXIT_FAILURE;
  }
  close(fd);

  // Open the file for reading
  fd = open(filename, O_RDONLY);
  if (fd == -1) {
    perror("open for reading");
    return EXIT_FAILURE;
  }

  // Start timer
  clock_gettime(CLOCK_MONOTONIC, &startTime);

  // Perform reads
  for (int i = 0; i < ITERATIONS; i++) {
    read(fd, buffer, 0);
  }

  // End timer
  clock_gettime(CLOCK_MONOTONIC, &endTime);

  // Calculate and print results
  long long total_elapsed_ns = elapsed_nanoseconds(startTime, endTime);
  long long average_ns = total_elapsed_ns / ITERATIONS;

  printf("Performed %d iterations.\n", ITERATIONS);
  printf("Total time: %lld ns\n", total_elapsed_ns);
  printf("Average cost of a 0-byte read() system call: %lld ns\n", average_ns);

  // Cleanup
  close(fd);
  if (remove(filename) != 0) {
    perror("remove");
  }

  return EXIT_SUCCESS;
}
