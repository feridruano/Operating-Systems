/***
 *
 * This example implements a pipeline-like ("|") facility to feed output of one command to another.
 *
 * The two commands are given as command line arguments to this program.
 *
 * Any of the command can have its own command line arguments by surrounding the command and the arguments
 * double quotes; for example, "ls -l -i" is considered a single argument, and it happens to be
 * a command with two of its own arguments.
 *
 */
#include	<stdio.h>
#include	<unistd.h>
#include	<sys/types.h>
#include	<stdlib.h>
#include	<string.h>

#define	oops(m,x)	{ perror(m); exit(x); }

#define STDIN fileno(stdin)    // same as if 0 used
#define STDOUT fileno(stdout)  // same as if 1 used

#define READ p[0]
#define WRITE p[1]

/***
 *
 *  Splits a string into an array of strings that are substrings of the string
 *  separated by any of the delimiters.
 *
***/
char** split(char *s, char* delim)
{
   char** arr = malloc(sizeof(char*));
   arr[0] = s;
   int i = 1;
   while (strsep(&s, delim) != NULL)
   {
      if (realloc(arr, (i+1) * sizeof(char*)) == NULL)
          exit(EXIT_FAILURE);

      arr[i++] = s;
   }

   if (realloc(arr, (i+1) * sizeof(char*)) == NULL)
       exit(EXIT_FAILURE);

   arr[i] = NULL;

   return arr;
}

int main(int argc, char **argv)
{
	int p[2]; // unidirectional pipe from command_1 to command_2
	int pid;

	char **command;

   if ( argc != 3 ){
		fprintf(stderr, "usage: pipe_2 command_1 command_2\n");
		exit(EXIT_FAILURE);
	}
	if ( pipe( p ) == -1 )
		oops("Cannot get a pipe", 1);
	
	if ( (pid = fork()) == -1 )
		oops("Cannot fork", 2);
	
	if ( pid > 0 ) // parent will be morphed into command_2
	{
		close(WRITE);	// command_2 will write to stdout, and not to the pipe, so we can close the WRITE end
		
		if ( dup2(READ, STDIN) == -1 )  // connect stdin to the pipe
			oops("could not redirect stdin to the pipe",3);
		
		close(READ); // READ end of the pipe has been duplicated as stdin, so we can close it

      command = split(argv[2], " ");
		execvp(command[0], command); // morph this process into command_2
		oops(argv[2], 4);
	}
	
	// child will be morphed into command_1
	
	close(READ); // command_1 will not read
	
	if ( dup2(WRITE, STDOUT) == -1 ) // writing will be to stdout
		oops("could not redirect stdout to the pipe", 4);
	
	close(WRITE); // WRITE end of the pipe has been duplicated as stdout, so we can close it

	command = split(argv[1], " ");
	execvp(command[0], command); // morph this process into command_1
	oops(argv[1], 5);
}