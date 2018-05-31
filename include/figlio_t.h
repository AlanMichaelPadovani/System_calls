#ifndef FIGLIO_T_H
#define FIGLIO_T_H

#include <pthread.h>
#include "utility.h"
#include <signal.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include "nipote_t.h"

int sem_out;
struct sembuf sembuf_out;

struct Nipote nipote1;
struct Nipote nipote2;
struct Nipote nipote3;
pthread_mutex_t lock1;
pthread_mutex_t lock2;

void figlio_t(int sem_out);
void status_updated();
void send_terminate();

#endif
