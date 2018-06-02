#include "../../include/thread/utility_t.h"

void initialize_thread(pthread_mutex_t * lock1, pthread_mutex_t * lock2, void * (*start_routine)(void *)){
    int i=0;
    struct Nipote nipoti[NUM_THREAD]; //list of struct
    pthread_t threads[NUM_THREAD]; //list of thread
    //popolate structs
    for(i; i<NUM_THREAD; ){
        nipoti[i].lock1=lock1;
        nipoti[i].lock2=lock2;
        nipoti[i].figlio=pthread_self();
        nipoti[i++].id=i;
    }
    //start threads
    for(i=0; i<NUM_THREAD; i++){
        pthread_create(&threads[i],NULL,start_routine,(void *) &(nipoti[i])); //check error creating
    }
    //wait threads
    for(i=0; i<NUM_THREAD; i++){
        pthread_join(threads[i],NULL);
    }
}

void start_threads(unsigned int * plain_text, unsigned int * encoded_text, unsigned int * key){
    struct Thread args[NUM_THREAD_KEY];
    pthread_t threads[NUM_THREAD_KEY];
    //inizialize lock
    pthread_mutex_init(&end_thread,NULL);
    //acquire lock
    pthread_mutex_lock(&end_thread);
    unsigned int start=0;
    int i=0;
    int step=(UINT_MAX/NUM_THREAD_KEY);
    for(i; i<NUM_THREAD_KEY-1; i++){
        args[i].plain_text=plain_text;
        args[i].encoded_text=encoded_text;
        args[i].key=key;
        args[i].start=start;
        args[i].end=start+step;
        args[i].lock=&end_thread;
        start=start+step+1;
        pthread_create(&threads[i],NULL,find_key_thread,(void *) &(args[i])); //check error creating

    }
    args[i].plain_text=plain_text;
    args[i].encoded_text=encoded_text;
    args[i].key=key;
    args[i].start=start;
    args[i].end=UINT_MAX;
    args[i].lock=&end_thread;
    pthread_create(&threads[i],NULL,find_key_thread,(void *) &(args[i]));
    //now all thread are started
    pthread_mutex_lock(&end_thread); //this make it wait until the thread release lock
    //void * cancel;
    //key is found, kill all the remaining thread
    for(i=0; i<NUM_THREAD_KEY; i++){
        if(pthread_cancel(threads[i])==0){
            //cancellation success
            /*
            pthread_join(threads[i], &cancel);
            if(cancel== PTHREAD_CANCELED){
                printf("successfully canceled\n");
            }else{
                printf("Thread wasn't canceled\n");
            }
            */
            printf("Canceled a thread\n");
        }else{
            printf("Can't cancel a thread\n");
        }
    }
    pthread_mutex_destroy(&end_thread);
}

void * find_key_thread(void * pointer){
    struct Thread me = * ((struct Thread *) pointer);
    long int key;
    for (key = me.start; key <= me.end; key++){
		if ((*(me.plain_text) ^ key) == *(me.encoded_text)){
            *(me.key) = (unsigned int) key;
            //release semaphore
            pthread_mutex_unlock(me.lock);
			pthread_exit(NULL);
		}
	}
    pthread_exit(NULL);
}
void stop(int seconds){
    //register for signal handler
    signal(SIGALRM,no_op);
    //set alarm
    alarm(seconds);
    //wait for alarm
    pause();
    if(alarm(0)!=0){
        //time is not over, re-set alarm
        return stop(seconds);
    }
}

void no_op(int s){
    return;
}

void * get_space(struct Memory * info, int byte){
    //ask at the OS for memory
    int id=shmget((*info).key,byte,IPC_CREAT|0666);
    (*info).id=id;
    //link this memory area to my address space
    if(id ==-1){
        perror(ERROR_GENERIC);
        _exit(EXIT_FAILURE);
    }
    (*info).pointer=shmat(id,NULL,0);
    return (*info).pointer;
}

int rem_space(struct Memory * info){
    if(shmdt((*info).pointer)==-1){
        perror(ERROR_GENERIC);
        _exit(EXIT_FAILURE);
    }
    return shmctl((*info).id,IPC_RMID,(struct shmid_ds *) NULL);
}
