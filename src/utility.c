#include "../include/utility.h"

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
