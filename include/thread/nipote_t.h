#ifndef NIPOTE_T_H
#define NIPOTE_T_H

#include <pthread.h>
#include "../utility.h"
#include <signal.h>
#include <sys/msg.h>
#include <limits.h>
#include <time.h>
#include <stdio.h>

struct sembuf sb;
unsigned int key;
pthread_mutex_t end_thread;

void * nipote_t(void * pointer);
int load_string();
unsigned int find_key(char * S1, int my_string, pthread_mutex_t * lock);
void send_timeelapsed(int seconds);
void save_key(unsigned int key, int offset);

void start_threads(unsigned int * plain_text, unsigned int * encoded_text, unsigned int * key);
void * find_key_thread(void * pointer);

#endif
