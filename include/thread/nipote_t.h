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

/// @defgroup var_comuni Variabili comuni per Nipote_t
/// @{
/// Variabili comuni per thread Nipote_t

/// la chiave trovata
unsigned int key;
/// lock per segnalare la fine della ricerca
pthread_mutex_t end_thread;
/// @}

/// @defgroup Metodi_Nipote_t Metodi Nipote_t
/// @{
/// Funzioni del processo Nipote_t

/**
 * @brief Wrapper per processo Nipote_t
 * @param pointer: Puntatore alla struttura contente informazioni necessarie alla thread
 */
void * nipote_t(void * pointer);

/**
 * @brief legge la stringa dal segmento S1
 * @return puntatore alla stringa successiva nella memoria condivisa S1
 */
int load_string();


/**
 * @brief Cerca la chiave della stringa
 * @param S1: La zona di memoria in cui è presente il file di input
 * @param my_string: L'indice della riga che deve elaborare
 * @param lock: Il lock condiviso tra le thread
 * @return la chiave usata per cifrare la riga
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
 * @brief Crea altre thread per dividere la ricerca delle chiavi
 * @param plain_text: Puntatore alla parte di testo in chiaro
 * @param encoded_text: Puntatore alla parte di testo cifrata
 * @param key: Puntatore alla zona di memoria dove andrà inserita la chiave trovata
 * @return
 */
void start_threads(unsigned int * plain_text, unsigned int * encoded_text, unsigned int * key);

/**
 * @brief Metodo che trova la chiave
 * @param pointer: La struttura con le informazioni necessarie per la thread
 */
void * find_key_thread(void * pointer);

/// @}

#endif
