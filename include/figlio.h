#ifndef FIGLIO_H
#define FIGLIO_H

#include "utility.h"
#include <signal.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include "nipote.h"

void figlio();
void status_updated();
void send_terminate();

#endif
