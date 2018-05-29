#include "../include/logger.h"

void logger(){
    //create message queue
    int msgid=-1;
    if((msgid=msgget(MSG_KEY,(0666|IPC_CREAT|IPC_EXCL)))==-1){
        perror(ERROR_GENERIC);
        _exit(EXIT_FAILURE);
    }
    polling_receive(msgid);
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
            perror(ERROR_GENERIC);
        	_exit(EXIT_FAILURE);
        }
        num_msg=info.msg_qnum;
        while(num_msg>0){
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

    }
}
