/*
1. Write a program that calls fork(). Before calling fork(), have the
main process access a variable (e.g., x) and set its value to something
(e.g., 100). What value is the variable in the child process?
What happens to the variable when both the child and parent change
the value of x?
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    int x = 100;

    int rc = fork();
    if (rc == -1) {
        // Fork failed
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        // child (new process)
        printf("Child: %d\n", x);
        x = 10;
        printf("Updated x - Child: %d\n", x);
    } else {
        // parent (main)
        printf("Parent %d\n", x);
        x = 10;
        printf("Updated x - Parent %d\n", x);
    }
    
    return 0;
}
