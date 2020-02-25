/**
* Name: Ferid Ruano
* Lab/task: Lab 04 - Task 2
* Date: 02/22/2020
**/

// Invocation: "$ ./monitor 200 4 25 50 75 100"
// [program | initMonitorTemp | numOfNodes | nodeTemps...]

// Node Invocation: "$ execlp("./node", "node", nodeIdStr, argv[i + 3], NULL);"

#include "message.h"

int main(int argc, char* argv[])
{
	// Minimum Require Arguments
	if (argc < 4)
	{
		puts("Usage: monitor <initial_monitor_temperature> <number_of_nodes> <node_1_temp> ...\n");
		exit(EXIT_FAILURE);
	}

	// Remove persisted message queues
	if (mq_unlink(MONITOR_QUEUE) == 0)
		printf("Message queue %s removed.\n", MONITOR_QUEUE);

	// Monitor Data
	TEMPERATURE monitor;
	monitor.previousTemperature = strtof(argv[INIT_MONITOR_TEMP_ARG], NULL);
	long numOfNodes = strtol(argv[NUM_OF_NODES_ARG], NULL, 10);
	TEMPERATURE nodeData[numOfNodes]; // Node Data Collection

	// Message Queue attributes
	struct mq_attr attr;
	attr.mq_maxmsg = numOfNodes;
	attr.mq_msgsize = MESSG_SIZE;
	attr.mq_curmsgs = 0;
	attr.mq_flags = 0;

	// Monitor - Incoming Message Queue
	if ((monitor.msqid = mq_open(MONITOR_QUEUE, O_RDWR | O_CREAT, S_IWUSR | S_IRUSR, &attr)) == -1)
	errExit("mq_open", errno);

	// Monitor - Outgoing Message Queues
	for (int i = 0; i < numOfNodes; ++i)
	{
		char name[NAME_MAX];
		sprintf(name, "/%s%d", NODE_NAME_PREFIX, (i + 1)); // Stack smashing is possible
		if ((nodeData[i].msqid = mq_open(name, O_RDWR | O_CREAT, S_IWUSR | S_IRUSR, &attr)) == -1)
		errExit ("mq_open", errno);

		pid_t pid;
		char id[NAME_MAX] = "";
		sprintf(id, "%d", (i + 1)); // Stack smashing is possible

		// Fork and Execute Node Processes
		switch (pid = fork())
		{
		case -1:
		errExit("Fork Failed", errno);
		case 0:
			execlp("./node", "node", id, argv[i + 4], NULL);
			break;
		default:
			break;
		}

		if (pid == 0)
			break;
	}

	MESSG MSG_RCVD;
	MESSG MSG_SENT;
	float sumOfNodeTemps;
	int numOfStable = 0;

	// Monitor - Regular Operation
	while (true)
	{
		sumOfNodeTemps = 0;

		// Retrieve Temperature Reports From All Nodes
		for (int i = 0; i < numOfNodes; ++i)
		{
			if (mq_receive(monitor.msqid, (char*)&MSG_RCVD, MESSG_SIZE, DEF_PRIO) == -1)
			errExit("mq_receive", errno);

			nodeData[i].msqid = MSG_RCVD.nodeId;
			sumOfNodeTemps += MSG_RCVD.temperature;
			nodeData[i].previousTemperature = MSG_RCVD.temperature;
		}

		// new_integrated_temp = (2 * previous_integrated_temp + sum_of_client_temps) / 6;
		monitor.previousTemperature = (2 * monitor.previousTemperature + sumOfNodeTemps) / 6;
		printf("MONITOR TEMPERATURE: %f", monitor.previousTemperature);

		for (int j = 0; j < numOfNodes; ++j)
		{
			MSG_SENT.nodeId = nodeData[j].msqid;
			MSG_SENT.temperature = monitor.previousTemperature;

			// Same number receives as sends
			if ((mq_send(nodeData[j].msqid, (const char*)&MSG_SENT, MESSG_SIZE, DEF_PRIO)) == -1)
			errExit("mq_send", errno);

			// Stability Check
			if (nodeData[j].previousTemperature == monitor.previousTemperature)
				numOfStable++;
		}

		if (numOfStable == numOfNodes)
			break;
	}

	puts("STABLE TEMPERATURE DETECTED.\nMONITOR TERMINATING...\n");

	// Send Stable Flag to All Nodes
	for (int k = 0; k < numOfNodes; ++k)
	{
		MSG_RCVD.nodeId = nodeData[k].msqid;
		MSG_RCVD.stable = true;
		if ((mq_send(nodeData[k].msqid, (const char*)&MSG_RCVD, MESSG_SIZE, DEF_PRIO)) == -1)
		errExit("mq_send", errno);
	}

	// Remove persisted message queues
	if (mq_close(monitor.msqid) == -1)
	errExit("mq_unlink", errno);

	// Remove persisted message queues
	if (mq_unlink(MONITOR_QUEUE) == -1)
	errExit("mq_unlink", errno);

	return 0;
}

// node[i].mqid
// stable check
