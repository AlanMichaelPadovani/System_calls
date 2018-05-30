#ifndef FIGLIO_T_H
#define FIGLIO_T_H

#include <pthread.h>
#include "utility.h"
#include <signal.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include "nipote_t.h"

struct Nipote nipote1;
struct Nipote nipote2;
struct sembuf sb;
int p;

void figlio_t();
void status_updated();
void send_terminate();

#endif
