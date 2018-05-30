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
void lock(int sem);
void unlock(int sem);
unsigned int find_key(char * S1, int my_string, int sem);
void send_timeelapsed(int seconds);
void save_key(unsigned int key, int offset);

#endif
