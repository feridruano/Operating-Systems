#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include	<unistd.h>
#include	<time.h>

#define SHM_NAME "MY_SHM"
#define SHM_SIZE 64

#define oops(m,x)  { perror(m); exit(x); }
