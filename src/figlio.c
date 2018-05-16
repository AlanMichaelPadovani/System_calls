#include "../include/figlio.h"

int figlio(){
	int p;

	//status_updated​ signal handler of signal SIGUSR1
	signal(SIGUSR1, status_updated);

	//create semaphore p
	p = semget(KEY_P, 1, IPC_CREAT|IPC_EXCL|0666);
	if (p == -1){
        //error semget
		printf("error creating p");
       	return 1;
	}


	//remove semaphore p
	int remove = semctl(KEY_P, 0, IPC_RMID, NULL);
	if (remove == -1){
		//error remove
		printf("error removing p\n");
       	return 1;
	}

	return 0;
}

void status_updated(){}

void send_terminate(){}

