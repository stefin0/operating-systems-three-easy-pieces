/*
2. Write a program that opens a file (with the open() system call)
and then calls fork() to create a new process. Can both the child
and parent access the file descriptor returned by open()? What
happens when they are writing to the file concurrently, i.e., at the
same time?
*/
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    int fd = open("./question02.output", O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);

    int rc = fork();

    if (rc < 0) {
        // Fork failed
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        // Child
        printf("Child: fd=%d\n", fd);
        write(fd, "Child writing\n", strlen("Child writing\n"));
    } else {
        // Parent
        printf("Parent: fd=%d\n", fd);
        write(fd, "Parent writing\n", strlen("Parent writing\n"));
    }

    return 0;
}
