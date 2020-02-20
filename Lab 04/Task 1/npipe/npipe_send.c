/***
 *
 * This is the server side of a demonstration of using named pipes for IPC.
 *
 */
#include	"npipe.h"

int main(int argc, char** argv) {
	char line[MAX_LINE];
	int pipe;

	int stat;
	if ((stat = mkfifo("/tmp/ajsFIFO", 0777)) < 0)
		if (errno != EEXIST)
			oops("Cannot make FIFO", stat);

	// open a named pipe
	pipe = open("/tmp/ajsFIFO", O_WRONLY );

	while (1)
	{
		// get a line to send
		printf("Enter line: ");
		fflush(stdout);
		fgets(line, MAX_LINE, stdin);
		*strchr(line, '\n') = '\0';
		if (strcmp(line, "quit") == 0)
			break;

		// actually write out the data and close the pipe
		write(pipe, line, strlen(line));
	}
	// close the pipe
	close(pipe);
	exit(EXIT_SUCCESS);
}
