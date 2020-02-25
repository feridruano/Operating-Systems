/**
* Name: Ferid Ruano
* Lab/task: Lab 04 - Task 2
* Date: 02/22/2020
**/

#include "message.h"

// Invocation: "$ ./node 1 100"
// [program | nodeId | initTemp]


int main(int argc, char* argv[])
{
	// Minimum Require Arguments
//	if (argc < 3)
//	{
//		puts("Usage: node <node_id> <temperature>\n");
//		exit(EXIT_FAILURE);
//	}

	// Open Outgoing Message Queue
	mqd_t monitor;
	if ((monitor = mq_open(MONITOR_QUEUE,  O_WRONLY) == -1))
	errExit("mq_open", errno);

	// Configure Message Description for Node
	char name[NAME_MAX] = "";
	sprintf(name, "/%s%ld", NODE_NAME_PREFIX,
		strtol(argv[NODE_ID_ARG], NULL, 10)); // Stack smashing is possible

	// Open Incoming Message Queue
	TEMPERATURE node;
	if ((node.msqid = mq_open(name,  O_RDWR | O_CREAT, S_IWUSR | S_IRUSR) == -1))
	errExit("mq_open", errno);

	// Retrieve Initial Temperature
	node.previousTemperature = strtof(argv[NODE_TEMP_ARG], NULL);

	MESSG MSG_RCVD;
	MESSG MSG_SENT;
	MSG_SENT.nodeId = node.msqid;

	// Regular Operation
	while (true)
	{
		MSG_RCVD.temperature = node.previousTemperature;

		// Send Temperature to Monitor
		if ((mq_send(monitor, (const char*)&MSG_SENT, MESSG_SIZE, DEF_PRIO)) == -1)
		errExit("mq_send (node.c)", errno);

		// Retrieve New Temperature from Monitor
		if ((mq_receive(node.msqid, (char*)&MSG_RCVD, MESSG_SIZE, DEF_PRIO) == -1))
		errExit("mq_receive", errno);

		if (MSG_RCVD.stable == 1)
			break;

		// new_node_temp = (previous_node_temp * 3 + 2 * new_integrated_temp) / 5;
		node.previousTemperature = (node.previousTemperature * 3 + 2 * MSG_RCVD.temperature) / 5;

		printf("NODE %s TEMPERATURE: %f\n", argv[NODE_ID_ARG], node.previousTemperature);
	}

	printf("NODE %f TERMINATING...", node.previousTemperature);

	// Close Incoming Message Queue
	if (mq_close(node.msqid) == -1)
	errExit("mq_close", errno);

	// Unlink Incoming Message
	if (mq_unlink(name) == -1)
	errExit("mq_unlink", errno);

	return 0;
}
