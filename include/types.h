#ifndef TYPES_H
#define TYPES_H

/**
 * @struct Status
 * @brief Struttura che mantiene l'identificatore del nipote e dell'indice della stringa che sta elaborando
*/
struct Status{
    /** identificatore del nipote che ha fatto ultimo accesso alla struttura */
    int grandson;
    /** numero di riga che sta attualmente elaborando il nipote */
    int id_string;
};

/**
 * @struct Message
 * @brief Struttura di un messaggio
*/
struct Message{
    /** intero che rappresenta il tipo di messaggio: \n
    1 - messaggio di terminazione \n
    2 - un nipote ha terminato una elaborazione */
    long mtype;
    /** contenuto del messaggio */
    char text[128];
};

/**
 * @struct Memory
 * @brief Struttura che contiene la chiave e l'identificatore del segmento di memoria e il puntatore alla zona di memoria stessa
*/
struct Memory{
    /** chiave della zona di memoria */
    key_t key;
    /** identificatore della zona di memoria */
    int id;
    /** indirizzo della prima zona di memoria */
    void * pointer;
};

/**
 * @struct Nipote
 * @brief Struttura utilizzata dalla thread Nipote che processa una stringa
*/
struct Nipote{
    /** identificatore del nipote */
    int id;
    /** lock per la prima zona di memoria */
    pthread_mutex_t * lock1;
    /** lock per la seconda zona di memoria */
    pthread_mutex_t * lock2;
    /** thread id del figlio */
    pthread_t figlio;
};

/**
 * @struct Thread
 * @brief Struttura contenente i dati utilizzati dalle thread per la ricerca della chiave
*/
struct Thread{
    /** testo in chiaro */
    unsigned int * plain_text;
    /** testo cifrato */
    unsigned int * encoded_text;
    /** puntatore alla zona di memoria in cui salvare la chiave trovata */
    unsigned int * key;
    /** la prima chiave da verificare */
    unsigned int start;
    /** l'ultima chiave da verificare */
    unsigned int end;
    /** lock condiviso tra le thread */
    pthread_mutex_t * lock;
};

#endif
