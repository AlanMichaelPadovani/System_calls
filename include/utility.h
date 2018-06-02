#ifndef UTILITY_H
#define UTILITY_H

//KEYS
#define KEY_S1 13352
#define KEY_S2 352
#define MSG_KEY 33352
#define KEY_P 432
#define KEY_SEM_WRITE 532

//MESSAGES
#define ERROR_GENERIC "Error"
#define ERROR_INPUT_PARAMETERS "Error: I file di input e di output sono parametri obbligatori.\n"
#define SIZE_ERROR_INPUT_PARAMETERS 63
#define ERROR_CHECK_KEYS "Error: Una delle chiavi trovate non è corretta per l'intera stringa.\n"
#define SIZE_ERROR_CHECK_KEYS 69

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> //alarm, pause and _exit
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include "types.h"

int * S1;
int * S2;
int num_line_inputfile;
int sem_write, sem_mem;
struct sembuf sb;

typedef int bool;
#define true 1
#define false 0

void stop(int seconds);
void no_op(int s);
void * get_space(struct Memory * info, int byte);
int rem_space(struct Memory * info);
void lock_semaphore(int sem, int num_sem);
void unlock_semaphore(int sem, int num_sem);

#endif
