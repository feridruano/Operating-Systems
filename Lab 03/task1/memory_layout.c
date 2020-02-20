/**
* Name: Ferid Ruano
* Lab/task: Lab 03
* Date: 02/11/2020
**/

#include <stdio.h>
#include <stdlib.h>
int error_type;
char *error_msg = "ERROR";
int main(int argc, char **argv)
{
int *val;
val = (int *) malloc(5 * sizeof(int));
for (int i = 0; i < 5; i++)
val[i] = i;
return EXIT_SUCCESS;
}
