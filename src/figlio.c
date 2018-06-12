#include "../include/figlio.h"

void figlio(){
	//status_updated​ signal handler of signal SIGUSR1
	signal(SIGUSR1, status_updated);

	//create semaphores p
	sem_mem = semget(KEY_P, 2, IPC_CREAT|IPC_EXCL|0666);
	if(sem_mem==-1){
		perror(ERROR_GENERIC);
        _exit(EXIT_FAILURE);
	}
	sb.sem_flg=0;
	//initialize first semaphore for S1
	unlock_semaphore(sem_mem, 0);
	//initialize second semaphore for S2
	unlock_semaphore(sem_mem, 1);

	//create nephew
	pid_t nipote1;
	if((nipote1=fork())==-1){
		//error creating first nephew
		perror(ERROR_GENERIC);
        _exit(EXIT_FAILURE);
	}if(nipote1==0){
		//nephew 1
		nipote(1);
	}else{
		//son
		//create nephew
		pid_t nipote2;
		if((nipote2=fork())==-1){
			//error creating second nephew
			perror(ERROR_GENERIC);
        	_exit(EXIT_FAILURE);
		}if(nipote2==0){
			//nephew 2
			nipote(2);
		}else{
			//son
			wait(NULL); //wait for a nephew
			wait(NULL); //wait for the other nephew
			send_terminate();

			//remove semaphores p
			if (semctl(sem_mem, 0, IPC_RMID, NULL) == -1){
				perror(ERROR_GENERIC);
        		_exit(EXIT_FAILURE);
			}

			_exit(EXIT_SUCCESS);
		}
	}
}

void status_updated(){
	//acquire semafore for S1
	lock_semaphore(sem_mem, 0);
	int * temp = S1; //save S1 into a temp variable
	//read from S1
	int grandson=*(S1++);
	S1=temp; //restore S1
	//set in ascii
	grandson=grandson+48;
	int id_string=*S1;
	//release semafore for S1
	unlock_semaphore(sem_mem, 0);
	id_string=id_string+48;
	char message1[]="Il nipote ";
	char message2[]=" sta analizzando la ";
	char message3[]="-esima stringa.\n";

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
		//error obtain message queue
		perror(ERROR_GENERIC);
        _exit(EXIT_FAILURE);
	}
	struct Message msg;
	msg.mtype=1;
	char text[]="ricerca conclusa\n";
	int index=0;
	char * pointer=&text[0];
	//insert message text
	while((*pointer)!='\0'){
		msg.text[index++]=(*(pointer++));
	}
	msg.text[index]='\0';
	//send message
	if(msgsnd(msgid,&msg,sizeof(msg)-sizeof(long),0)==-1){
		perror(ERROR_GENERIC);
        _exit(EXIT_FAILURE);
	}
}
