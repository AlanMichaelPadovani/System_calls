#include "../include/nipote.h"

int nipote(int sem, int * S1){
    int my_string;
    sb.sem_num=0;
	sb.sem_flg=0;
    //acquire semaphore
    lock(sem);
    //CRITICAL SECTION
    //access status
    my_string=load_string(++S1);
    printf("My string is: %d\n",my_string);
    //END CRITICAL SECTION
    unlock(sem);
    return 0;
}

int load_string(int * S1){
    //S1 points at the next string
    return (*S1);
}

int lock(int sem){
    //acquire the semaphore
	sb.sem_op=-1;
	if(semop(sem,&sb,1)==-1){
		//error acquiring semaphore
		return 1;
	}
    return 0;
}

int unlock(int sem){
    //release the semaphore
    sb.sem_op=1;
    if(semop(sem,&sb,1)==-1){
        //error acquiring semaphore
        return 1;
    }
    return 0;
}

int find_key();
void send_timeelapsed();
void save_key();
