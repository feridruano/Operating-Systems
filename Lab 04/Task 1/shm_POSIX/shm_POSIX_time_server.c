/***
 *
 * This is a demonstration of using a shared memory for IPC.
 *
 * This is a time server that will feed the current time to connecting clients.
 *
 */
#include "shm_POSIX.h"

int main()
{
    long now; // time now
    int fd; // shared memory file descriptor

    char *mem_ptr; // pointer to shared memory object

    // create the shared memory object
    if ((fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, S_IWUSR | S_IRUSR)) == -1)
        oops("CANNOT OPEN SHM", EXIT_FAILURE);

    ftruncate(fd, SHM_SIZE); // cut down to the desired size

    // memory map the shared memory object
    mem_ptr = (char *) mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if (mem_ptr == MAP_FAILED)
        oops("CANNOT MAP THE MEMORY: ", EXIT_FAILURE);

   // run a clock for a minute
    printf("Time ticks: ");
    for (int i = 0; i < 60; i++)
    {
        fflush(stdout);
        time(&now);                   // get the time
        strcpy(mem_ptr, ctime(&now)); // write to memory
        sleep(1);                     // wait a sec
        printf(".");
    }
    printf("\n");

    shm_unlink(SHM_NAME); // remove the shared memory object

    exit(EXIT_SUCCESS);
}