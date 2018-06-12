#ifndef NIPOTE_T_H
#define NIPOTE_T_H

/// @file

#include <pthread.h>
#include "../utility.h"
#include <signal.h>
#include <sys/msg.h>
#include <limits.h>
#include <time.h>
#include <errno.h>

unsigned int key;
pthread_mutex_t end_thread;

/// @defgroup Metodi_Nipote_t
/// @{
/// Funzioni del processo Nipote_t

/**
 * @brief Wrapper per processo Nipote_t
 * @param pointer: TODO
 */
void * nipote_t(void * pointer);

/**
 * @brief legge la stringa dal segmento S1
 * @return puntatore alla stringa successiva nella memoria condivisa S1
 */
int load_string();


/**
 * @brief TODO
 * @return 
 */
unsigned int find_key(char * S1, int my_string, pthread_mutex_t * lock);

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

/**
 * @brief TODO
 * @return 
 */
void start_threads(unsigned int * plain_text, unsigned int * encoded_text, unsigned int * key);

/**
 * @brief TODO
 * @return 
 */
void * find_key_thread(void * pointer);

/// @}

#endif
