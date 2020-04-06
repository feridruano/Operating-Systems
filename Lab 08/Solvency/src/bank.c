/**
* Name: Ferid Ruano
* Lab/task: Lab 08
* Date: 04/05/2020
**/

#include "bank.h"

int numOfCustomers;        // the number of customers of the bank
int numOfAccounts;         // the number of accounts offered by the bank

double* available;    // the amount available of each customer
double** maximum;     // the maximum demand of each customer
double** allocation;  // the amount currently allocated to each customer
double** need;     // the remaining needs of each customer

/***
 * Read the state of the bank from a file.
 *
 * The file format is as follows:
 *
 * <num of customers> <num of accounts>
 * <initial state of accounts>
 * <max needs for each customer>
 *
 * e.g.,
 *
 * 5 3
 * 15 10 5
 * 7,5,3
 * 3,2,2
 * 9,0,2
 * 2,2,2
 * 4,3,3
 *
 * Any of  ", \n\t" is a valid separator
 *
 */
void loadBankState()
{
	// create the bank
	initBank();

	// read initial values for maximum array
	for (int i = 0; i < numOfCustomers; i++)
		addBankCustomer(i);
}

/***
 * Initialize the bank for a number of accounts and customers.
 */
void initBank()
{
	double in[2];
	readLine(in);
	numOfCustomers = (int)in[0];
	numOfAccounts = (int)in[1];

	available = calloc(numOfAccounts, sizeof(double));
	readLine(available);

	// initialize the accounts
	maximum = calloc(numOfCustomers, sizeof(double*));
	allocation = calloc(numOfCustomers, sizeof(double*));
	need = calloc(numOfCustomers, sizeof(double*));
}

/***
 * This function adds a customer to the bank system.
 * It records its maximum fund demand with the bank.
 */
void addBankCustomer(int customerNum)
{
	maximum[customerNum] = calloc(numOfAccounts, sizeof(double));
	readLine(maximum[customerNum]);

	allocation[customerNum] = calloc(numOfAccounts, sizeof(double));
	// we start with zero allocated

	need[customerNum] = calloc(numOfAccounts, sizeof(double));
	arraycpy(need[customerNum], maximum[customerNum], numOfAccounts);
}

/***
 * Outputs the state of the bank; i.e., funds in each account of each customer
 */
void displayBankState()
{
	printf("%13s", "Available =");
	display(available, numOfAccounts);

	printf("\n%13s", "Allocation =");
	for (int i = 0; i < numOfCustomers; i++)
		display(allocation[i], numOfAccounts);

	printf("\n%13s", "Max =");
	for (int i = 0; i < numOfCustomers; i++)
		display(maximum[i], numOfAccounts);

	printf("\n%13s", "Need =");
	for (int i = 0; i < numOfCustomers; i++)
		display(need[i], numOfAccounts);

	printf("\n");
}

/***
 * Determines whether the currect system state is safe.
 */
bool isSafeState(int customerNum, double* request)
{
	// TODO - Done
	//	1. Let Work and Finish be vectors of length m and n, respectively. Initialize
	//		Work = Available and Finish[i] = false for i = 0, 1, ..., n − 1.
	double work[numOfAccounts];
	// arraycpy(work, available, numOfAccounts);
	bool finish[numOfCustomers];
	set(false, finish, numOfCustomers);

	// Have the system pretend to have allocated the requested resources to
	// thread T i by modifying the state as follows:
	//		Available = Available–Request i
	// 		Allocation i = Allocation i + Request i
	// 		Need i = Need i –Request i
	sub2(work, available, request, numOfAccounts);
	add(allocation[customerNum], request, numOfAccounts);
	sub(need[customerNum], request, numOfAccounts);

	//	2. Find an index i such that both
	//		a. Finish[i] == false
	//		b. Need i ≤ Work
	//		If no such i exists, go to step 4.
	//	3. Work = Work + Allocation i
	//		Finish[i] = true
	//		Go to step 2.

	for (int i = 0; i < numOfCustomers; ++i)
	{
		if (false == finish[i] && lessOrSame(need[i], work, numOfAccounts))
		{
			add(work, allocation[i], numOfAccounts);
			finish[i] = true;
		}
	}

	//	4. If Finish[i] == true for all i, then the system is in a safe state.
	//		This algorithm may require an order of m × n 2 operations to determine whether
	//		a state is safe.
	bool ret = allTrue(finish, numOfCustomers);
	if (true == ret)
	{
		printf("\nThe customers that could be satisfied (in order):");
		for (int i = 0; i < numOfCustomers; ++i)
			if (finish[i])
				printf(" %d", i);
	}

	// Restore Original Resource Allocations
	sub(allocation[customerNum], request, numOfAccounts);
	add(need[customerNum], request, numOfAccounts);

	return ret;
}

/***
 * Determines whether a request for loan can be granted.
 */
bool solvencyTest(int customerNum, double* request)
{
	// TODO - Done
	//	1. If Request i ≤ Need i , go to step 2. Otherwise, raise an error condition, since
	//	the thread has exceeded its maximum claim.
	if (lessOrSame(request, need[customerNum], numOfAccounts))
	{
		//	2. If Request i ≤ Available, go to step 3. Otherwise, T i must wait, since the
		//		resources are not available.
		if (lessOrSame(request, available, numOfAccounts))
			if (isSafeState(customerNum, request))
				return true;
	}
	else
		puts("\nCustomer cannot ask for more assets than the allowed maximum.");

	return false;
}

/***
 * Make a request for a loan.
 */
bool borrow(int customerNum, double funds[])
{
	// TODO: Done
	bool ret = solvencyTest(customerNum, funds);

	if (true == ret)
	{
		// Take from Available Resources
		sub(available, funds, numOfAccounts);
		// Take from Remaining Customer Resources
		sub(need[customerNum], funds, numOfAccounts);
		// Allocate Resources to Customer
		add(allocation[customerNum], funds, numOfAccounts);
	}

	return ret;
}

/***
 * Repay a loan.
 */
bool repay(int customerNum, double funds[])
{
	// TODO: Done
	bool ret = lessOrSame(funds, allocation[customerNum], numOfAccounts);

	if (true == ret)
	{
		// Take from Allocated Customer Resources
		sub(allocation[customerNum], funds, numOfAccounts);
		// Update Remaining Resources
		add(need[customerNum], funds, numOfAccounts);
		// Return Customer Resources to Available Resources
		add(available, funds, numOfAccounts);
	}
	else
		puts("\nCustomer cannot return funds that are not allocated currently.");

	return ret;
}

/***
 * In a loop, take input from the standard console until 'Q' or 'q' is entered.
 *
 * Each input should have the following syntax:
 *
 * {Q[UIT] | ST[ATUS] | [BO[RROW] | RE[PAY]] <customer number>  {<account funds> ...}}
 *
 * e.g. (for 3 accounts and with at least 2 customers):
 *
 * ST
 * BO 1 3 2 1
 * RE 1 1 0 1
 * QUIT
 *
 */
void bankTeller()
{
	// now loop reading requests to withdraw or deposit funds
	double request[numOfAccounts];
	char* inp = malloc(MAX_LINE_SIZE * sizeof(char));
	while (true)
	{
		printf("\nTeller: How may I help you? > ");

		if (fgets(inp, MAX_LINE_SIZE, stdin) == 0)
			break;

		if (strlen(inp) == 0)
			goto exception;

		// need a copy, since "line" will be used for tokenizing, so
		// it will change until it gets NULL; we would nnot be able to re-use it
		char* line = strdup(inp);

		// get transaction type - borrow (BO) or repay (RE) or ST (status)
		line[strlen(line) - 1] = '\0';

		if (line == NULL)
			goto exception;

		char* trans = strsep(&line, " ,\t");
		strToUpper(&trans); // so the case does not matter

		printf("REQUEST: %s\n", trans);
		if (trans[0] == 'Q')
		{
			displayBankState();
			exit(1);
		}
		else if (strncmp(trans, "STATUS", 2) == 0)
			displayBankState();
		else
		{
			if ((strncmp(trans, "BORROW", 2) != 0) && (strncmp(trans, "REPAY", 2) != 0))
				goto exception;

			// get the customer number making the tranaction
			if (line == NULL)
				goto exception;

			int custNum = strtod(strsep(&line, " "), NULL);
			printf("CLIENT %d: ", custNum);

			if (custNum >= numOfCustomers)
				goto exception;

			// get the resources involved in the transaction
			for (int i = 0; i < numOfAccounts; i++)
			{
				if (line == NULL)
					goto exception;

				request[i] = strtod(strsep(&line, " ,\t"), NULL);
			}

			display(request, numOfAccounts);

			// now check the transaction type
			if (strncmp(trans, "BORROW", 2) == 0)
			{
				if (borrow(custNum, request))
					printf("\n*APPROVED*\n");
				else
					printf("\n*DENIED*\n");
			}
			else if (strncmp(trans, "REPAY", 2) == 0)
			{
				if (repay(custNum, request))
					printf("\n*APPROVED*\n");
				else
					printf("\n*DENIED*\n");
			}
			continue;
		exception:
			printf("\nExpected input: Q[UIT] | ST[ATUS] | [BO[RROW] | RE[PAY]] <customer number> <resource #1> <#2> <#3> ...\n");
			continue;
		}
	}
}

/***
 * Read a line of values separated by a set of delimeters from a file into an array
 */
void readLine(double array[])
{
	char* line = malloc(MAX_LINE_SIZE * sizeof(char));
	fgets(line, MAX_LINE_SIZE, stdin);
	char* tok;
	int i = 0;
	while ((tok = strsep(&line, ", \n\t")) != NULL)
		if (strlen(tok) > 0)
			array[i++] = strtod(tok, NULL);

	free(line);
}

/***
 * Convert the string to upper case
 */
void strToUpper(char** s)
{
	for (char* c = *s; *c; c++)
		if (isascii(*c) && islower(*c))
			*c = toupper(*c);
}
