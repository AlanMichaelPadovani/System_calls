#ifndef LIB_H
#define LIB_H

#define KEY_S1 1034
#define KEY_S2 2034
#define MSG_KEY 30334
#define KEY_P 4034

#include <signal.h>
#include <stdio.h>
#include <unistd.h> //alarm and pause
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include "types.h"

int * S1;
int * S2;
int num_line_inputfile;

typedef int bool;
#define true 1
#define false 0

void stop(int seconds);
void no_op(int s);
void * get_space(struct Memory * info, int byte);
int rem_space(struct Memory * info);

#endif