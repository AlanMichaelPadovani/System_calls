#include "../../include/thread/figlio_t.h"

void figlio_t(){
	//status_updated​ signal handler of signal SIGUSR1
	signal(SIGUSR1, status_updated);

	//inizialize locks
	if (pthread_mutex_init(&lock1, NULL) != 0){
    	perror(ERROR_GENERIC);
    	_exit(EXIT_FAILURE);
    }
    if (pthread_mutex_init(&lock2, NULL) != 0){
    	perror(ERROR_GENERIC);
    	_exit(EXIT_FAILURE);
    }
	
	//create nephew thread
	initialize_thread(&lock1, &lock2, nipote_t);
	
	//remove locks
	if (pthread_mutex_destroy(&lock1) != 0){
    	perror(ERROR_GENERIC);
    	_exit(EXIT_FAILURE);
    }
    if (pthread_mutex_destroy(&lock2) != 0){
    	perror(ERROR_GENERIC);
    	_exit(EXIT_FAILURE);
    }
	send_terminate();
	_exit(EXIT_SUCCESS);
}

void status_updated(){
	if (pthread_mutex_lock(&lock1) != 0){
    	perror(ERROR_GENERIC);
    	_exit(EXIT_FAILURE);
    }
	
	int * temp = S1; //save S1 into a temp variable
	//read from S1
	int grandson=*(S1++);
	//set in ascii
	grandson=grandson+48;
	int id_string=*S1;
	S1=temp; //restore S1
	if (pthread_mutex_unlock(&lock1) != 0){
    	perror(ERROR_GENERIC);
    	_exit(EXIT_FAILURE);
    }
	id_string=id_string+48;
	char message1[]="Il nipote ";
	char message2[]=" sta analizzando la ";
	char message3[]=" -esima stringa.\n";
	
	//acquire semaphore for stdout
    lock_semaphore(sem_write, 0);
	
	write(1,&message1,sizeof(message1));
	write(1,&grandson,sizeof(int));
	write(1,&message2,sizeof(message2));
	write(1,&id_string,sizeof(int));
	write(1,&message3,sizeof(message3));
	
	//release semaphore for stdout
    unlock_semaphore(sem_write, 0);
}

void send_terminate(){
	int msgid;
	if((msgid=msgget(MSG_KEY,0666)) == -1){
		perror(ERROR_GENERIC);
        _exit(EXIT_FAILURE);
	}
	struct Message msg;
	msg.mtype=1;
	char text[]="ricerca conclusa\n";
	int index=0;
	char * pointer=&text[0];
	while((*pointer)!='\0'){
		msg.text[index++]=(*(pointer++));
	}
	msg.text[index]='\0';
	if(msgsnd(msgid,&msg,sizeof(msg)-sizeof(long),0)==-1){
		perror(ERROR_GENERIC);
        _exit(EXIT_FAILURE);
	}
}

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
        if (pthread_create(&threads[i],NULL,start_routine,(void *) &(nipoti[i])) != 0){
        	perror(ERROR_GENERIC);
        }
    }
    //wait threads
    for(i=0; i<NUM_THREAD; i++){
        if (pthread_join(threads[i],NULL) != 0){
        	perror(ERROR_GENERIC);
        }
    }
}
