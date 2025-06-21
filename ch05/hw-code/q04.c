/*
 * 4. Write a program that calls fork() and then calls some form of
 *    exec() to run the program /bin/ls. See if you can try all of the
 *    variants of exec(), including (on Linux) execl(), execle(),
 *    execlp(), execv(), execvp(), and execvpe(). Why do
 *    you think there are so many variants of the same basic call?
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
  // call fork()
  pid_t pid_fork = fork();
  if (pid_fork < 0) { // fork failed
    fprintf(stderr, "fork failed\n");
    exit(EXIT_FAILURE);
  } else if (pid_fork == 0) { // child
    // call some form of exec() to run /bin/ls

    // execl()
    printf("running execl()\n");
    execl("/bin/ls", "ls", "-l", NULL);

    // execvp()
    // char *myargs[2];
    // myargs[0] = strdup("ls");
    // myargs[1] = NULL;
    // execvp(myargs[0], myargs);
  } else { // parent
    pid_t pid_wait = wait(NULL);
  }

  return EXIT_SUCCESS;
}
