#ifndef FIGLIO_H
#define FIGLIO_H

#define KEY_P 14

#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include "types.h"

int figlio();
void status_updated();
void send_terminate();

#endif
