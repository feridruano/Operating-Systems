/**
* Name: Ferid Ruano
* Lab/task: Lab 05
* Date: 02/26/2020
**/

#include "matrixMult.h"

int main(int argc, char* argv[])
{
	// Redirect Standard Input to File
	if (freopen(argv[1], "r", stdin) == 0)
	oops("Cannot open the input file.\n", -1);

	int** a1, ** b1, ** c1, ** a2, ** b2, ** c2; // Matrices
	int m1, k1, n1, m2, k2, n2; // Dimensions of the matrices m x k and k x n

	// Allocate Memory and Load Matrices
	allocateAndLoadMatrices(&a1, &b1, &c1, &m1, &k1, &n1);
	allocateAndLoadMatrices(&a2, &b2, &c2, &m2, &k2, &n2);

	// TODO: Done
	pthread_t** tids;
	tids = multiply(a1, b1, c1, m1, k1, n1);
	join(tids, m1, n1);
	free_tids(tids, m1);

	tids = multiply(a2, b2, c2, m2, k2, n2);
	join(tids, m2, n2);
	free_tids(tids, m2);

	// Display Results of Matrix Multiplication
	printf("\nMATRIX A1\n");
	displayMatrix(a1, m1, k1);
	freeMatrix(a1, m1);
	printf("\nMATRIX B1\n");
	displayMatrix(b1, k1, n1);
	freeMatrix(b1, k1);
	printf("\nMATRIX A1 x B1\n");
	displayMatrix(c1, m1, n1);
	freeMatrix(c1, m1);

	// Display Results of Matrix Multiplication
	printf("\nMATRIX A2\n");
	displayMatrix(a2, m2, k2);
	freeMatrix(a2, m2);
	printf("\nMATRIX B2\n");
	displayMatrix(b2, k2, n2);
	freeMatrix(b2, k2);
	printf("\nMATRIX A2 x B2\n");
	displayMatrix(c2, m2, n2);
	freeMatrix(c2, m2);

	return 0;
}

void* matrixThread(void* param)
{
	MATRIX_CELL* cell = (MATRIX_CELL*)param;

	// TODO: Done
	for (int i = 0; i < cell->k; ++i)
		cell->c[cell->i][cell->j] += cell->a[cell->i][i] * cell->b[i][cell->j];

	free(cell);
	pthread_exit(NULL);
}

void allocateAndLoadMatrices(int*** a, int*** b, int*** c, int* m, int* k, int* n)
{
	// Takes Pointers to Two-Dimensional Matrices to be Allocated and Used by the Caller
	if (scanf("%d %d %d", m, k, n) == 0)
	oops("Cannot read matrix sizes.\n", -2);

	// TODO: Done

	// Check for Valid Matrix Multiplication
	if (*m < 1 || *k < 1 || *n < 1)
	oops("Invalid M, K, or N.\n", -2);

	// Allocate Memory for Matrices
	*a = (int**)malloc(*m * sizeof(int*));
	*b = (int**)malloc(*k * sizeof(int*));
	*c = (int**)malloc(*m * sizeof(int*));
	for (int i = 0; i < *m; ++i)
	{
		(*a)[i] = (int*)malloc(*k * sizeof(int));
		(*c)[i] = (int*)malloc(*n * sizeof(int));
	}
	for (int i = 0; i < *k; ++i)
		(*b)[i] = (int*)malloc(*n * sizeof(int));

	// Load Matrices
	loadMatrix(a, *m, *k);
	loadMatrix(b, *k, *n);
}

void loadMatrix(int*** matrix, int m, int n)
{
	// TODO: Done
	for (int i = 0; i < m; ++i)
		for (int j = 0; j < n; ++j)
			scanf("%d", &((*matrix)[i][j]));
}

void freeMatrix(int** matrix, int m)
{
	// TODO: Done
	for (int i = 0; i < m; ++i)
		free(matrix[i]);
	free(matrix);
}

pthread_t** multiply(int** a, int** b, int** c, int m, int k, int n)
{
	pthread_t** tids = alloc_tids(m, n);

	// TODO: Done
	// M * N Threads
	for (int i = 0; i < m; ++i)
		for (int j = 0; j < n; ++j)
		{
			MATRIX_CELL* cell = malloc(sizeof(MATRIX_CELL));
			cell->i = i;
			cell->j = j;
			cell->k = k;
			cell->a = a;
			cell->b = b;
			cell->c = c;
			pthread_create(&tids[i][j], NULL, matrixThread, (void*)cell);
		}
	return tids;
}

pthread_t** alloc_tids(int m, int n)
{
	pthread_t** tids;

	// TODO: Done
	tids = malloc(m * sizeof(pthread_t*));
	for (int i = 0; i < m; ++i)
		tids[i] = malloc(n * sizeof(pthread_t));
	return tids;
}

void free_tids(pthread_t** tids, int m)
{
	// TODO: Done
	for (int i = 0; i < m; ++i)
		free(tids[i]);
	free(tids);
}

void join(pthread_t** tids, int m, int n)
{
	// TODO: Done
	for (int i = 0; i < m; ++i)
		for (int j = 0; j < n; ++j)
			pthread_join(tids[i][j], NULL);
}

void displayMatrix(int** matrix, int m, int n)
{
	// TODO: Done
	for (int i = 0; i < m; ++i)
	{
		for (int j = 0; j < n; ++j)
			printf("%d ", matrix[i][j]);
		puts("\n");
	}
}
