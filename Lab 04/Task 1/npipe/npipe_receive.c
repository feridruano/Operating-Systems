/***
 *
 * This is the receiver side of a demonstration of using named pipes for IPC.
 *
 */
#include        "npipe.h"

int main(int argc, char** argv) {
	char line[MAX_LINE];
	int n, pipe;

	// open a named pipe
	pipe = open("/tmp/ajsFIFO", O_RDONLY);

	// set the mode to blocking (note '~')
	int flags = 0;
	flags &= ~O_NONBLOCK;
	//flags |= O_NONBLOCK;
	fcntl(pipe, F_SETFL, flags);

	// read the data from the pipe
	n = read(pipe, line, MAX_LINE);
	line[n] = '\0';

	printf("%d bytes received: %s\n", n, line);

	// close the pipe
	close(pipe);
    exit(EXIT_SUCCESS);
}
