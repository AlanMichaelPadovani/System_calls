#ifndef PADRE_H
#define PADRE_H

/// @file

#include "utility.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include "logger.h"
#include "figlio.h"

/// @defgroup Metodi_Padre Metodi Padre
/// @{
/// Funzioni del processo Padre

/**
 * @brief Wrapper per processo Padre
 * @param input_path: puntatore al nome del file di input
 * @param output_path: puntatore al nome del file di output
 */
void padre(char * input_path, char * output_path);

/**
 * @brief crea segmento di memoria condivisa
 * @param key: chiave del segmento da creare
 * @param size: dimensione del segmento da creare
 * @param shmid: TODO
 * @return puntatore al segmento di memoria condivisa creato
 */
int * attach_segments(int key, int size, int * shmid);

/**
 * @brief ​elimina segmento di memoria condivisa
 * @param locate: puntatore al segmento di memoria condivisa
 * @param shmid: TODO
 * @return return value della system call invocata
 */
int detach_segments(void * locate, int * shmid);

/**
 * @brief ​carica il file di input
 * @param fd: file descriptor del file in input
 * @param S1: puntatore al segmento di memoria condivisa S1 in cui verrà memorizzato il contenuto del file
 * @param save: TODO
 */
void load_file(int fd, char * S1, int * * save);

/**
 * @brief ​salva le chiavi sul file di output
 * @param output_path: puntatore al nome del file di output
 * @param S2: puntatore al segmento di memoria condivisa S2 in cui sono memorizzate le chiavi trovate
 */
void save_keys(char * output_path, unsigned int * S2);

/**
 * @brief controlla che le chiavi trovate siano corrette per tutta la lunghezza delle relative stringhe
 * @param num_keys: numero di chiavi trovate, corrispondente al numero di stringhe nel file di input
 * @param S1: puntatore al segmento di memoria condivisa S1 in cui sono memorizzate stringhe del file di input
 * @param S2: puntatore al segmento di memoria condivisa S2 in cui sono memorizzate le chiavi trovate
 * @return true se le chiavi trovate sono corrette per le intere stringhe, false altrimenti
 */
bool check_keys(int num_keys, char * S1, char * S2);

/// @}

#endif
