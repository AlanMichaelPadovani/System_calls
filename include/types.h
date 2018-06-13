#ifndef TYPES_H
#define TYPES_H

/**
 * @struct Status
 * @brief Struttura che mantiene l'identificatore del nipote e dell'indice della stringa che sta elaborando
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
 * @brief Struttura che contiene la chiave e l'identificatore del segmento di memoria e il puntatore alla zona di memoria stessa
*/
struct Memory{
    key_t key; /**DESCRIZIONE A PARTE SOTTO*/
    int id; ///DESCRIZIONE SUBITO SOTTO
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
