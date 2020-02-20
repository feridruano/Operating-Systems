/**
* Name: Ferid Ruano
* Lab/task: Lab 03
* Date: 02/11/2020
**/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
int main()
{
pid_t pid;
/* fork a child process */
pid = fork();
if (pid < 0) { /* error occurred */
fprintf(stderr, "Fork Failed\n");
exit(EXIT_FAILURE);
}
else if (pid == 0) { /* child process */
printf("I am the child %d\n", getpid());
execlp("/bin/ls", "ls", NULL); // run /bin/ls in the child
exit(EXIT_FAILURE); // executed only if execlp() fails
}
else { /* parent process; pid is the pid of the child */
/* parent will wait for the child to complete */
printf("I am the parent %d\n", getpid());
waitpid(pid, NULL, 1); // wait for the child with the given pid
printf("Child Complete\n");
exit(EXIT_SUCCESS);
}
}
