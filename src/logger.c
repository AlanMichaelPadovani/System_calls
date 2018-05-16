#include "../include/logger.h"

int logger(){
    //create message queue

    int msgid=-1;
    if((msgid=msgget(MSG_KEY,(0666|IPC_CREAT)))==-1){
        //error
        printf("noo");
        return 1;
    }
    polling_receive(msgid);
    if(msgctl(msgid,IPC_RMID,(struct msqid_ds *) NULL)==-1) printf("Impossibile eliminare coda di messaggi\n");
    return 0;
}
void polling_receive(int msgid){
    struct Message message;
    int flag=1;
    while(1){
        alarm(0);//delete prev alarm
        alarm(1); //set alarm
        pause();
        alarm(0);
        msgrcv(msgid, &message, sizeof(struct Message) - sizeof(long), 0,IPC_NOWAIT);
        if(alarm(0)!=0){
            //exited for another signal
        }
        flag=0;
    }
}
