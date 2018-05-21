#ifndef LOGGER_H
#define LOGGER_H


#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include "types.h"
#include "lib.h"

int logger(int msgkey);
void polling_receive(int msgid);

#endif
