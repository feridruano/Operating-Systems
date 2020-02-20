/***
 *
 * This is the sender side of a demonstration of using POSIX message queues for IPC.
 *
 */
#include "ipc_POSIX_messg.h"

int main()
{
   mqd_t msqid;
   unsigned int type;

   struct mq_attr attr;

   char *msg = calloc(MSG_SIZE, sizeof(char));
   strcpy(msg, "Hello receiver!");

   if ((msqid = mq_open(QUEUE_NAME, O_RDWR | O_CREAT, S_IRWXU | S_IRWXG, NULL)) < 0)
      oops("Error opening a queue.", errno);

   if (mq_getattr(msqid, &attr) == -1)
      oops("Error getting queue attributes.", errno);

   printf("Queue opened with:\n");
   printf("Maximum # of messages on queue:   %ld\n", attr.mq_maxmsg);
   printf("Maximum message size:             %ld\n", attr.mq_msgsize);

   // We'll send message type 1 (arbitrarily)
   if (mq_send(msqid, msg, strlen(msg) + 1, MSG_TYPE) < 0)
      oops("Error opening a queue.", errno);

   printf("Sent: \"%s\"\nRun the receiver.\n", msg);

   printf("To continue, press Enter on the keybord...");
   getchar(); // just to stop an dwait for the receiver to get the message

   if (mq_receive(msqid, msg, MSG_SIZE, &type) < 0)
      oops("Error receiving data.", errno);

   printf("Received: \"%s\" of type %d\n", msg, type);

   mq_close(msqid);
   mq_unlink(QUEUE_NAME);

   exit(EXIT_SUCCESS);
}
