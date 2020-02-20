/***
 *
 * This program implements a facility to execute another program in a child process,
 * send it a message, and obtain a response.
 *
 * For example, it could be used to execute some action by a surrogate process.
 *
 */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define STDIN            fileno(stdin) // same as using 0
#define STDOUT           fileno(stdout) // same as using 1

#define PARENT_READ      read_pipe[0]
#define PARENT_WRITE     write_pipe[1]
#define CHILD_READ       write_pipe[0]
#define CHILD_WRITE      read_pipe[1]

#define BUF_SIZE 1024

int main()
{
    /** Pipe for reading from subprocess */
    int read_pipe[2];
    /** Pipe for writing to subprocess */
    int write_pipe[2];

    char *buffer = calloc(BUF_SIZE, 1);

    if (pipe(read_pipe) == 0 && pipe(write_pipe) == 0)
    {
        pid_t pid = fork();
        if (pid == (pid_t) - 1)
        {
            fprintf(stderr, "Fork failure");
            exit(EXIT_FAILURE);
        }
        else if (pid == (pid_t)0) //Child process
        {
            // close the standards since the pipe will be used instead
            close(STDIN);
            close(STDOUT);

            // close the parent side of the pipe (since we are in the child process)
            close(PARENT_READ);
            close(PARENT_WRITE);

            // redirect stdin (0) and stdout (1) to the pipe
            dup2(CHILD_READ, STDIN);
            dup2(CHILD_WRITE, STDOUT);
            // the stdin is from the parent now
            // the stdout points to the parent now

            // run "cat" that takes input from stdin and echoes it to stdout on eol
            // (try it from a terminal)
            execlp("cat", "cat", (char *)NULL);

            // man bc it
            // execlp("bc", "bc", "-l", (char *)NULL);

            exit(EXIT_FAILURE);
        }
        else  //Parent process
        {
            // close the child's side of the pipe (since we are in the parent process)
            close(CHILD_READ);
            close(CHILD_WRITE);

            char *expr = "This is PARENT speaking!"; // for cat
            // char *expr = "scale=10\npi=4*a(1)\npi\n"; // for bc
            write(PARENT_WRITE, expr, strlen(expr));
            int r = read(PARENT_READ, buffer, BUF_SIZE);
            printf("%d %d\n", r, errno);
            puts(buffer);
        }
    }

    exit(EXIT_SUCCESS);
}
