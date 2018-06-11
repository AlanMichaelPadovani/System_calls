#ifndef FIGLIO_H
#define FIGLIO_H

/// @file

#include "utility.h"
#include <signal.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include "nipote.h"

/// @defgroup Metodi_Figlio
/// @{
/// Funzioni del processo Figlio

/**
 * @brief Wrapper per processo Figlio
 */
void figlio();

/**
 * @brief Signal handler
 */
void status_updated();

/**
 * @brief Deposita il messaggio di terminazione nella coda di messaggi del processo Logger
 */
void send_terminate();

/// @}

#endif
