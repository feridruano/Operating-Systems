#ifndef __BANK_H_
#define __BANK_H_

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <pthread.h>
#include "arrays.h"

#define MAX_LINE_SIZE 256

void initBank();

void addBankCustomer(int);

void displayBankState();

bool borrow(int, double[]);

bool repay(int, double[]);

void loadBankState();

void bankTeller();

void readLine(double[]);

void strToUpper(char **);

#endif