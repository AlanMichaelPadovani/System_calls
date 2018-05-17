#ifndef FIGLIO_H
#define FIGLIO_H

#define KEY_P 12

#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include "types.h"
#include <unistd.h>
#include "nipote.h"

int * S1;
struct sembuf sb;
int p;

int figlio(int * S1v);
void status_updated();
void send_terminate();

#endif
