/**
* Name: Ferid Ruano
* Lab/task: Lab 04 - Task 2
* Date: 02/22/2020
**/

#ifndef LAB04_MESSAGE_H
#define LAB04_MESSAGE_H

// Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#include <mqueue.h>

// Definitions
#define MONITOR_QUEUE "/MONITOR"
#define NODE_NAME_PREFIX "NODE_"

#define MESSG_SIZE sizeof(MESSG)
#define INIT_MONITOR_TEMP_ARG 1
#define NUM_OF_NODES_ARG 2
#define NODE_TEMP_ARG 2
#define NODE_ID_ARG 1
#define DEF_PRIO 0
#define errExit(ermsg,erno) {perror(ermsg); exit(erno); }

// Message Attributes
typedef struct messg {
    bool stable;
    int nodeId;
    float temperature;
} MESSG;

// Monitor Data Collection
typedef struct temperature {
    mqd_t msqid;
    float previousTemperature;
} TEMPERATURE;

#endif //LAB04_MESSAGE_H
