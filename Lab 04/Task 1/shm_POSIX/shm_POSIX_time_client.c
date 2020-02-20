/***
 *
 * This is a demonstration of using a shared memory for IPC.
 *
 * This is a time client that connects to a time server to obtain the current time.
 *
 */
#include "shm_POSIX.h"

int main()
{
	int fd; // shared memory file descriptor
	char *mem_ptr; // pointer to shared memory obect

	// open the shared memory object
	if ((fd = shm_open(SHM_NAME, O_RDONLY, 0400)) == -1)
		oops("CANNOT OPEN SHM", EXIT_FAILURE);

	// memory map the shared memory object
	mem_ptr = (char *) mmap(0, SHM_SIZE, PROT_READ, MAP_SHARED, fd, 0);

	if (mem_ptr == MAP_FAILED)
		oops("CANNOT MAP THE MEMORY: ", EXIT_FAILURE);

    // read from the shared memory object
	printf("TIME NOW: %s\n", mem_ptr);

	munmap(mem_ptr, SHM_SIZE);

    exit(EXIT_SUCCESS);
}