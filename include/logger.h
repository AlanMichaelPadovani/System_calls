#ifndef LOGGER_H
#define LOGGER_H

#include "utility.h"
#include <sys/msg.h>

void logger();
void polling_receive(int msgid);

#endif
