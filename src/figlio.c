#include "../include/figlio.h"

void figlio(int out){
	sem_out=out;
	sb.sem_flg=0;
	//status_updated​ signal handler of signal SIGUSR1
	signal(SIGUSR1, status_updated);
	//create semaphores p
	p = semget(KEY_P, 2, IPC_CREAT|IPC_EXCL|0666);
	sb.sem_num=0;
	sb.sem_op=1;
	//initialize first semaphore for S1
	if(p == -1 || semop(p,&sb,1)==-1){
		perror(ERROR_GENERIC);
        _exit(EXIT_FAILURE);
	}

	sb.sem_num=1;
	sb.sem_op=1;
	//initialize second semaphore for S2
	if(semop(p,&sb,1)==-1){
		perror(ERROR_GENERIC);
        _exit(EXIT_FAILURE);
	}
	sb.sem_num=0; //set sb to first semaphore for signal
	//create nephew
	pid_t nipote1;
	if((nipote1=fork())==-1){
		perror(ERROR_GENERIC);
        _exit(EXIT_FAILURE);
	}if(nipote1==0){
		//nephew 1
		nipote(1,p);
	}else{
		//son
		//create nephew
		pid_t nipote2;
		if((nipote2=fork())==-1){
			perror(ERROR_GENERIC);
        	_exit(EXIT_FAILURE);
		}if(nipote2==0){
			//nephew 2
			nipote(2,p);
		}else{
			//son
			wait(NULL); //wait for a nephew
			wait(NULL); //wait for the other nephew
			send_terminate();
			//remove semaphores p
			int remove = semctl(p, 0, IPC_RMID, NULL);
			if (remove == -1){
				perror(ERROR_GENERIC);
        		_exit(EXIT_FAILURE);
			}

			_exit(EXIT_SUCCESS);
		}
	}
}

void status_updated(){
	//acquire semafore for S1
	sb.sem_op=-1;
	if(semop(p,&sb,1)==-1){
		perror(ERROR_GENERIC);
        _exit(EXIT_FAILURE);
	}
	int * temp = S1; //save S1 into a temp variable
	//read from S1
	int grandson=*(S1++);
	//set in ascii
	grandson=grandson+48;
	int id_string=*S1;
	id_string=id_string+48;
	char message1[]="Il nipote ";
	char message2[]=" sta analizzando la ";
	char message3[]="-esima stringa.\n";
	//acquire semaphore for stdout
	if(semop(sem_out,&sb,1)==-1){
		//error acquiring semaphore
		perror(ERROR_GENERIC);
		_exit(EXIT_FAILURE);
	}
	write(1,&message1,sizeof(message1));
	write(1,&grandson,sizeof(int));
	write(1,&message2,sizeof(message2));
	write(1,&id_string,sizeof(int));
	write(1,&message3,sizeof(message3));
	//release semaphore for stdout
	sb.sem_op=1;
	if(semop(sem_out,&sb,1)==-1){
		//error releasing semaphore
		perror(ERROR_GENERIC);
		_exit(EXIT_FAILURE);
	}
	S1=temp; //restore S1
	//release semafore for S1
	//initialize first semaphore for S1
	if(semop(p,&sb,1)==-1){
		perror(ERROR_GENERIC);
        _exit(EXIT_FAILURE);
	}
}

void send_terminate(){
	int msgid;
	if((msgid=msgget(MSG_KEY,0666)) == -1){
		perror(ERROR_GENERIC);
        _exit(EXIT_FAILURE);
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
		perror(ERROR_GENERIC);
        _exit(EXIT_FAILURE);
	}
}
