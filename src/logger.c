#include "../include/logger.h"

void logger(){
    //create message queue
    int msgid=-1;
    if((msgid=msgget(MSG_KEY,(0666|IPC_CREAT|IPC_EXCL)))==-1){
        perror(ERROR_GENERIC);
        _exit(EXIT_FAILURE);
    }
    //start listening message
    polling_receive(msgid);
    //end research remove message queue
    if(msgctl(msgid,IPC_RMID,(struct msqid_ds *) NULL)==-1){
    	perror(ERROR_GENERIC);
        _exit(EXIT_FAILURE);
    }
    _exit(EXIT_SUCCESS);
}

void polling_receive(int msgid){
    struct Message message;
    struct msqid_ds info;
    int flag=1,num_msg;
    while(flag){
        stop(1); //wait for 1 sec
        if(msgctl(msgid,IPC_STAT, &info)==-1){
            //error obtaining message queue
            perror(ERROR_GENERIC);
        	_exit(EXIT_FAILURE);
        }
        num_msg=info.msg_qnum;
        //acquire semaphore for stdout
        lock_semaphore(sem_write, 0);
        //loop on messages
        while(num_msg>0){
            //read message
            msgrcv(msgid, &message, sizeof(struct Message) - sizeof(long), 0,IPC_NOWAIT);
            if(message.mtype == 1){
                //terminate
                write(1,&(message.text),17);
                //read remaining message
                flag=0;
                num_msg--;
            }else{
                //message from nephew
                int size=0;
                while(message.text[size++]!='\0');
                write(1,&(message.text),size);
                num_msg--;
            }
        }
        //release semaphore for stdout
        unlock_semaphore(sem_write, 0);
    }
}
