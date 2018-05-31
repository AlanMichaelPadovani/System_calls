#ifndef NIPOTE_H
#define NIPOTE_H

#include "utility.h"
#include <signal.h>
#include <sys/msg.h>
#include <limits.h>
#include <time.h>

struct sembuf sb;

void nipote(int id, int sem);
int load_string();
void lock(int sem, int num);
void unlock(int sem, int num);
unsigned int find_key(char * S1, int my_string, int sem);
void send_timeelapsed(int seconds);
void save_key(unsigned int key, int offset);

#endif
