/***
 *
 * This is a simple illustration of the mechanics behind opening a pipe to a child process.
 *
 */

#include	<sys/types.h>
#include	<stdio.h>
#include	<string.h>
#include	<unistd.h>
#include    <wait.h>

#define MAXLINE 1024
#define	oops(m,x)	{ perror(m); exit(x); }

int main(void)
{
	int		n, fd[2];
	pid_t	pid;
	char	line[MAXLINE];

	if (pipe(fd) < 0)
		printf("Cannot create a pipe.");

	if ( (pid = fork()) < 0)
		printf("Cannot fork.");

	else if (pid == 0) /* child */
	{
		write(fd[1], "Message for the parent\n", 23);
		n = read(fd[0], line, MAXLINE);
		printf("%d bytes read: %s", n, line);
		close(fd[1]);
	}
	else /* parent */
	{
		write(fd[1], "Message for the child\n", 22);
		n = read(fd[0], line, MAXLINE);
		printf("%d bytes read: %s", n, line);
		close(fd[0]);
		waitpid(-1, NULL, 0);
	}
}
