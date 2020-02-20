#ifndef _IPC_MESSG_H
#define _IPC_MESSG_H

#include <sys/types.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

#define oops(ermsg,erno) {perror(ermsg); exit(erno); }

#define QUEUE_NAME "/myQueue"

#define MSG_SIZE 8192

#define MSG_TYPE 1

#endif
