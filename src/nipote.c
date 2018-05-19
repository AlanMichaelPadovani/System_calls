#include "../include/nipote.h"

int nipote(int id, int sem, int * S1, int num_line){
    int my_string;
    sb.sem_num=0;
	sb.sem_flg=0;
    char my_str[1029];
    char * temp;
    //acquire semaphore
    while(1){
        lock(sem);
        //CRITICAL SECTION
        //access status
        my_string=load_string(++S1);
        if(my_string==num_line){
            S1--;
            //end nephew
            unlock(sem);
            return 0;
        }else{
            //increment id_string
            (*S1)++;
            //set grandson
            (*(--S1))=id;
            //notify son
            kill(getppid(),SIGUSR1);
            //copy my string
            //END CRITICAL SECTION
            unlock(sem);
            //sleep(4);
        }
    }
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
