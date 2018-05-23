#ifndef NIPOTE_H
#define NIPOTE_H

#include "utility.h"
#include <signal.h>
#include <stdlib.h> //sleep
#include <sys/msg.h>
#include <limits.h>
#include <time.h>//time

struct sembuf sb;

int nipote(int id, int sem);
int load_string();
int lock(int sem);
int unlock(int sem);
unsigned int find_key(char * S1, int my_string, int sem);
void send_timeelapsed(int seconds);
void save_key(unsigned int key, int offset);

#endif
