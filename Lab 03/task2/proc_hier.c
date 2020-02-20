/**
* Name: Ferid Ruano
* Lab/task: Lab 03 - Task 2
* Date: 02/16/2020
**/

#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>
#include    <unistd.h>
#include    <errno.h>
#include    <sys/types.h>
#include    <sys/wait.h>

#define oops(m) {perror(m); exit(EXIT_FAILURE);}

void child(char *tag) {
    pid_t pid = fork();
    if (pid < 0) {
        oops("Failed Fork");
    } else if (pid == 0) {
        execlp("./iam", "iam", tag, NULL);
    }
}

int main() {
    for (int i = 0; i < 2; ++i) {
        pid_t pid = fork();
        if (pid == 0) {
            if (i == 0) {
                child("1.1.1");
                child("1.1.2");
                execlp("./iam", "iam", "1.1", NULL);
            } else {
                child("1.2.1");
                child("1.2.2");
                execlp("./iam", "iam", "1.2", NULL);
            }
        } else if (pid < 0) {
            oops("Failed Fork");
        }
    }

    printf("I am the parent %d\n", getpid());
    if (waitpid(-1, NULL, 0) < 0)
        printf("-1 from wait() with errno = %d\n", errno);

    printf("Child terminated; parent exiting\n");
    exit(EXIT_SUCCESS);
}
