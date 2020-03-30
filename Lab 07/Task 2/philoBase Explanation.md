Name: Ferid Ruano

Lab/task: Lab 07

Date: 03/13/2020



philoBase.c Explanation

The initial number of seats is 7 with 8 turns. The program gets stuck due to a deadlock situation in which a thread locks a critical section of code and tries to access another critical section that has been locked by another thread. All other threads preform the same task and will run into an "infinite" wait period for another critical section to be unlocked.