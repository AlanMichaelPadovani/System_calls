#ifndef FIGLIO_T_H
#define FIGLIO_T_H

/// @file

#include <pthread.h>
#include "../utility.h"
#include <signal.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include "nipote_t.h"

pthread_mutex_t lock1;
pthread_mutex_t lock2;

/// @defgroup Metodi_Figlio_t
/// @{
/// Funzioni del processo Figlio_t

/**
 * @brief Wrapper per processo Figlio_t
 */
void figlio_t();

/**
 * @brief Signal handler
 */
void status_updated();

/**
 * @brief Deposita il messaggio di terminazione nella coda di messaggi del processo Logger
 */
void send_terminate();


/**
 * @brief Metodo che crea le thread nipoti
 * @param lock1: Un lock per proteggere la zona di memoria di S1
 * @param lock2: Un lock per proteggere la zona di memoria di S2
 * @param start_routine: Il metodo che verrà eseguito dalle thread
 */
void initialize_thread(pthread_mutex_t * lock1, pthread_mutex_t * lock2, void * (*start_routine)(void *));

/// @}

#endif
