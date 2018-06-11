#ifndef TYPES_H
#define TYPES_H

/**
 * @struct Status 
 * @brief TODO
*/
struct Status{
    int grandson;
    int id_string;
};

/**
 * @struct Message 
 * @brief Struttura di un messaggio
*/
struct Message{
    long mtype;
    char text[128];
};

/**
 * @struct Memory 
 * @brief TODO
*/
struct Memory{
    key_t key;
    int id;
    void * pointer;
};

/**
 * @struct Nipote 
 * @brief Struttura utilizzata dalla thread Nipote che processa una stringa
*/
struct Nipote{
    int id;
    pthread_mutex_t * lock1;
    pthread_mutex_t * lock2;
    pthread_t figlio;
};

/**
 * @struct Thread 
 * @brief Struttura contenente i dati utilizzati dalle thread per la ricerca della chiave
*/
struct Thread{
    unsigned int * plain_text;
    unsigned int * encoded_text;
    unsigned int * key;
    unsigned int start;
    unsigned int end;
    pthread_mutex_t * lock;
};

#endif
