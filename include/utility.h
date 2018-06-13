#ifndef UTILITY_H
#define UTILITY_H

/// @file


/*! \mainpage Sistemi Operativi 2017/2018
 *
 * \section struttura_sec Struttura Progetto
 *
 * \subsection step1 Versione base
 * blabla....
 * \subsection step2 Versione con thread 
 * etc...
 *
 */


#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> //alarm, pause and _exit
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include "types.h"

/// @defgroup Config_thread Parametri Configurazione thread
/// @{
/// Parametri di configurazione per versione con thread

/// Numero di thread Nipote
#define NUM_THREAD 2
/// Numero di thread per ricerca chiave
#define NUM_THREAD_KEY 2
/// @}

/// @defgroup Keys
/// @{
#define KEY_S1 50056211
#define KEY_S2 705031
#define MSG_KEY 3050321
#define KEY_P 596501
#define KEY_SEM_WRITE 4650011
/// @}

/// @defgroup Messaggi
/// @{
#define ERROR_GENERIC "Error"
#define ERROR_INPUT_PARAMETERS "Error: I file di input e di output sono parametri obbligatori.\n"
#define ERROR_CHECK_KEYS "Error: Una delle chiavi trovate non è corretta per l'intera stringa.\n"
/// @}
#define SIZE_ERROR_INPUT_PARAMETERS 63
#define SIZE_ERROR_CHECK_KEYS 69


/// @defgroup Var_comuni Variabili comuni
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

/// @defgroup Metodi_utility Metodi di utility
/// @{
/// Metodi comuni utilizzati in vari punti del codice

/**
 * @brief Metodo che implementa una wait
 * @param seconds: Il numero di secondi per cui il chiamante verrà bloccato
 */
void stop(int seconds);

/**
 * @brief Funzione che intercetta la fine del tempo di attesa
 * @param s: La signal intercettata
 */
void no_op(int s);

/**
 * @brief Metodo che crea un segmento di memoria condivisa
 * @param info: Il puntatore alla struttura che conterrà tutte le informazioni relative al segmento creato
 * @param byte: La dimensione in byte dello spazio richiesto
 */
void * get_space(struct Memory * info, int byte);

/**
 * @brief Metodo che elimina un segmento di memoria condivisa
 * @param info: Il puntatore alla struttura che contiene le informazioni della zona di memoria da eliminare
 * @return -1 in caso di errore
 */
int rem_space(struct Memory * info);

/**
 * @brief Acquire del semaforo passato in input
 * @param sem: semaforo
 * @param num_sem: numero del semaforo nell'insieme
 */
void lock_semaphore(int sem, int num_sem);

/**
 * @brief Release del semaforo passato in input
 * @param sem: semaforo
 * @param num_sem: numero del semaforo nell'insieme
 */
void unlock_semaphore(int sem, int num_sem);

/// @}

#endif
