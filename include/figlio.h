#ifndef FIGLIO_H
#define FIGLIO_H

#include "utility.h"
#include <signal.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include "nipote.h"

int sem_out,p;
struct sembuf sb;

void figlio(int sem_out);
void status_updated();
void send_terminate();

#endif
