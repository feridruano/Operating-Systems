/***
 *
 * This is the receiver side of a demonstration of using POSIX message queues for IPC.
 *
 */
#include "ipc_POSIX_messg.h"

int main()
{
   mqd_t msqid;
   char msg_rcvd[MSG_SIZE], msg_send[MSG_SIZE];
   unsigned int type;

   if ((msqid = mq_open(QUEUE_NAME, O_RDWR)) < 0)
      oops("Error opening a queue.", errno);

   if (mq_receive(msqid, msg_rcvd, MSG_SIZE, &type) < 0)
      oops("Error receiving data.", errno);

   printf("Received: \"%s\" of type %d\n", msg_rcvd, type);

   sprintf(msg_send, "%s", msg_rcvd);

   if (mq_send(msqid, msg_send, MSG_SIZE, MSG_TYPE) < 0)
      oops("Error responding.", errno);

   printf("Sent back: \"%s\"\n", msg_send);

   mq_close(msqid);

   exit(EXIT_SUCCESS);
}
