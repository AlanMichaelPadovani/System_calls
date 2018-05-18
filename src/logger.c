#include "../include/logger.h"

int logger(int msgkey){
    //create message queue
    printf("logger active\n");
    int msgid=-1;
    if((msgid=msgget(msgkey,(0666|IPC_CREAT|IPC_EXCL)))==-1){
        //error
        printf("Can't get message queue\n");
        return 1;
    }
    polling_receive(msgid);
    if(msgctl(msgid,IPC_RMID,(struct msqid_ds *) NULL)==-1) printf("Impossibile eliminare coda di messaggi\n");
    return 0;
}
void polling_receive(int msgid){
    struct Message message;
    int flag=1;
    while(flag){
        //alarm(1); //set alarm
        //pause();
        sleep(1); //wait for 1 sec
        msgrcv(msgid, &message, sizeof(struct Message) - sizeof(long), 0,IPC_NOWAIT);
        if(message.mtype == 1){
            //terminate

            write(1,&(message.text),17);
            //read remaining message
            flag=0;
        }else{
            //message from nephew
        }

    }
    return;
    /*

    int flag=1;
    while(flag==1){

        alarm(0);//delete prev alarm
        alarm(1); //set alarm
        pause();
        if(alarm(0)!=0){
            //exited for another signal
        }
        msgrcv(msgid, &message, sizeof(struct Message) - sizeof(long), 0,IPC_NOWAIT);
        if(message.mtype == 1){
            //terminate

            write(1,&(message.text),sizeof(message.text));
            //read remaining message

        }
        flag=0;
    }
    printf("Polling receive end\n");
    */
}
