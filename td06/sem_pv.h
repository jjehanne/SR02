#include <stdio.h>

#include <stdlib.h>

#include <unistd.h>

#include <sys/types.h>

#include <sys/ipc.h>

#include <sys/sem.h>

#define N_SEM 5

int init_semaphore(void);

int detruire_semaphore(void);

int val_sem(int, int);

int P(int);

int V(int); 
