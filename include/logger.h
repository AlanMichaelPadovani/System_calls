#ifndef LOGGER_H
#define LOGGER_H

#define MSG_KEY 134
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include "types.h"

int logger();
void polling_receive(int msgid);

#endif
