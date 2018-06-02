#ifndef UTILITY_T_H
#define UTILITY_T_H

//THREADS
#define NUM_THREAD 1
#define NUM_THREAD_KEY 3

//KEYS
#define KEY_S1 1052
#define KEY_S2 2052
#define MSG_KEY 30352
#define KEY_P 4052
#define KEY_P_OUT 5052

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
#include "../types.h"
#include <pthread.h>
#include <limits.h>

int * S1;
int * S2;
int num_line_inputfile;
pthread_mutex_t end_thread;
typedef int bool;
#define true 1
#define false 0

void stop(int seconds);
void no_op(int s);
void * get_space(struct Memory * info, int byte);
int rem_space(struct Memory * info);
void initialize_thread(pthread_mutex_t * lock1, pthread_mutex_t * lock2, void * (*start_routine)(void *));

void start_threads(unsigned int * plain_text, unsigned int * encoded_text, unsigned int * key);
void * find_key_thread(void * pointer);

#endif
