#ifndef NIPOTE_H
#define NIPOTE_H

/// @file

#include "utility.h"
#include <signal.h>
#include <sys/msg.h>
#include <limits.h>
#include <time.h>

/// @defgroup Metodi_Nipote
/// @{
/// Funzioni del processo Nipote


/**
 * @brief Wrapper per processo Nipote
 * @param id del processo nipote
 */
void nipote(int id);

/**
 * @brief legge la stringa dal segmento S1
 * @return puntatore alla stringa successiva nella memoria condivisa S1
 */
int load_string();

/**
 * @brief Acquire del semaforo passato in input - richiama il metodo lock_semaphore di Utility
 * @param sem: semaforo
 * @param num_sem: numero del semaforo nell'insieme
 */
void lock(int sem, int num);

/**
 * @brief Release del semaforo passato in input - richiama il metodo unlock_semaphore di Utility
 * @param sem: semaforo
 * @param num_sem: numero del semaforo nell'insieme
 */
void unlock(int sem, int num);

/**
 * @brief trova la chiave
 * @param S1: puntatore alla memoria condivisa S1
 * @param offset: offset per recuperare la stringa corrente nella memoria condivisa S1
 */
unsigned int find_key(char * S1, int my_string);

/**
 * @brief deposita nella coda di messaggi del processo Logger l'informazione del tempo impiegato nella ricerca della chiave
 * @param seconds: secondi impiegati
 */
void send_timeelapsed(int seconds);

/**
 * @brief deposita nella coda di messaggi del processo Logger l'informazione del tempo impiegato nella ricerca della chiave
 * @param key: chiave trovata
 * @param offset: offset per la memorizzazione della chiave nella memoria condivisa S2
 */
void save_key(unsigned int key, int offset);

/// @}

#endif
