#include "../include/figlio.h"

int figlio(int * S1v, int * S2, int msgkey, int num_line){
	S1=S1v;
	//status_updated​ signal handler of signal SIGUSR1
	signal(SIGUSR1, status_updated);

	//create semaphore p
	p = semget(KEY_P, 1, IPC_CREAT|IPC_EXCL|0666);
	sb.sem_num=0;
	sb.sem_op=1;
	sb.sem_flg=0;
	if(semop(p,&sb,1)==-1){
		//error initializing semaphore
		return 1;
	}
	if (p == -1){
        //error semget
		printf("error creating p");
       	return 1;
	}

	//create nephew
	pid_t nipote1;
	if((nipote1=fork())==-1){
		//error creating nephew 1
		return 1;
	}if(nipote1==0){
		//nephew 1
		nipote(1,p,S1,S2,num_line,msgkey);
	}else{
		//son
		//create nephew
		pid_t nipote2;
		if((nipote2=fork())==-1){
			//error creating nephew 2
			return 1;
		}if(nipote2==0){
			//nephew 2
			nipote(2,p,S1,S2,num_line,msgkey);
		}else{
			//son
			wait(NULL); //wait for a nephew
			wait(NULL); //wait for the other nephew
			send_terminate(msgkey);
			//remove semaphore p
			int remove = semctl(p, 0, IPC_RMID, NULL);
			if (remove == -1){
				//error remove
				printf("error removing p\n");
		       	return 1;
			}

			return 0;
		}
	}
}

void status_updated(){
	//acquire the semaphore
	/*
	sb.sem_op=-1;
	if(semop(p,&sb,1)==-1){
		//error acquiring semaphore
		return;
	}*/
	//CRITICAL SECTION
	int * temp = S1; //save S1 into a temp variable
	//read from S1
	int grandson=*(S1++);
	//set in ascii
	grandson=grandson+48;
	int id_string=*S1;
	id_string=id_string+48;
	char message1[]="Il nipote ";
	char message2[]=" sta analizzando la ";
	char message3[]=" -esima stringa.\n";
	write(1,&message1,sizeof(message1));
	write(1,&grandson,sizeof(int));
	write(1,&message2,sizeof(message2));
	write(1,&id_string,sizeof(int));
	write(1,&message3,sizeof(message3));
	S1=temp; //restore S1

	//END CRITICAL SECTION
	//release semaphore
	/*
	sb.sem_op=1;
	if(semop(p,&sb,1)==-1){
		//error relasing semaphore
		return;
	}*/
}

void send_terminate(int msgkey){
	int msgid;
	if((msgid=msgget(msgkey,0666)) == -1){
		//can't get the message queue
		return;
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
		//error sending terminate message
		return;
	}
	return;
}
