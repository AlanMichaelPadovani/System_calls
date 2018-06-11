#ifndef UTILITY_H
#define UTILITY_H

/// @file

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> //alarm, pause and _exit
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include "types.h"

/// @defgroup Config_thread
/// @{
/// Parametri di configurazione per versione con thread

/// Numero di thread Nipote
#define NUM_THREAD 2
/// Numero di thread per ricerca chiave
#define NUM_THREAD_KEY 2
/// @}

/// @defgroup Keys
/// @{
#define KEY_S1 621
#define KEY_S2 7211
#define MSG_KEY 821
#define KEY_P 421
#define KEY_SEM_WRITE 121
/// @}

/// @defgroup Messaggi
/// @{
#define ERROR_GENERIC "Error"
#define ERROR_INPUT_PARAMETERS "Error: I file di input e di output sono parametri obbligatori.\n"
#define SIZE_ERROR_INPUT_PARAMETERS 63
#define ERROR_CHECK_KEYS "Error: Una delle chiavi trovate non è corretta per l'intera stringa.\n"
#define SIZE_ERROR_CHECK_KEYS 69
/// @}


/// @defgroup Variabili_comuni
/// @{
/// Puntatore memoria condivisa S1
int * S1;
/// Puntatore memoria condivisa S2
int * S2;
/// Numero righe del file in input
int num_line_inputfile;
/// Semaforo per write su stdout
int sem_write;
/// Semaforo per accesso a memoria condivisa
int sem_mem;
/// Buffer semaforo
struct sembuf sb;
/// @}

typedef int bool;
#define true 1
#define false 0

/// @defgroup Metodi_utility
/// @{
/// Metodi comuni utilizzati in vari punti del codice

/**
 * @brief TODO
 * @param
 * @return
 */
void stop(int seconds);

/**
 * @brief TODOTODO
 * @param
 */
void no_op(int s);

/**
 * @brief TODO
 * @param
 * @param
 */
void * get_space(struct Memory * info, int byte);

/**
 * @brief TODO
 * @param
 */
int rem_space(struct Memory * info);

/**
 * @brief Acquire the semaphore passed as input
 * @param semaphore
 * @param number of semaphore
 */
void lock_semaphore(int sem, int num_sem);

/**
 * @brief Release the semaphore passed as input
 * @param semaphore
 * @param number of semaphore
 */
void unlock_semaphore(int sem, int num_sem);

/// @}

#endif
