#ifndef LIB_H
#define LIB_H

#include <signal.h>
#include <unistd.h> //alarm and pause
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "types.h"

void stop(int seconds);
void no_op(int s);
void * get_space(struct Memory * info, int byte);
int rem_space(struct Memory * info);

#endif
