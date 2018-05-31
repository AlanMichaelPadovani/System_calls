#ifndef NIPOTE_T_H
#define NIPOTE_T_H

#include <pthread.h>
#include "utility.h"
#include <signal.h>
#include <sys/msg.h>
#include <limits.h>
#include <time.h>
#include <stdio.h>

struct sembuf sb;

void nipote(int id, int sem);
void * nipote_t(void * pointer);
int load_string();
unsigned int find_key(char * S1, int my_string, pthread_mutex_t * lock);
void send_timeelapsed(int seconds);
void save_key(unsigned int key, int offset);

#endif
