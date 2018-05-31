#ifndef LOGGER_H
#define LOGGER_H

#include "utility.h"
#include <sys/msg.h>

void logger(int sem_out);
void polling_receive(int msgid, int sem_out);

#endif
