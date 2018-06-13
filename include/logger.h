#ifndef LOGGER_H
#define LOGGER_H

/// @file

#include "utility.h"
#include <sys/msg.h>

/// @defgroup Metodi_Logger
/// @{
/// Funzioni del processo Logger

/**
 * @brief Wrapper per processo Logger
 */
void logger();

/**
 * @brief scarica la coda di messaggi e la stampa su stdout
 * @param msgid: l'identificatore della coda di messaggi da monitorare
 */
void polling_receive(int msgid);

/// @}

#endif
