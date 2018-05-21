#ifndef NIPOTE_H
#define NIPOTE_H

#include <sys/sem.h>
#include <stdio.h> //printf
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h> //sleep
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <limits.h>
#include "types.h"
#include <time.h>//time

struct sembuf sb;

int nipote(int id, int sem, int * S1, int * S2, int num_line, int msgkey);
int load_string(int * S1);
int lock(int sem);
int unlock(int sem);
int find_key(char * S1, int my_string);
void send_timeelapsed(int msgkey, int seconds);
void save_key(char * S2, int key, int offset);

#endif
