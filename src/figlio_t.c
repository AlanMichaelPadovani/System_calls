#include "../include/figlio_t.h"

void figlio_t(){
	//status_updated​ signal handler of signal SIGUSR1
	signal(SIGUSR1, status_updated);

	//create semaphore p
	p = semget(KEY_P, 2, IPC_CREAT|IPC_EXCL|0666);
	sb.sem_num=0;
	sb.sem_op=1;
	sb.sem_flg=0;
	if(p == -1 || semop(p,&sb,1)==-1){
		perror(ERROR_GENERIC);
        _exit(EXIT_FAILURE);
	}

	sb.sem_num=1;
	sb.sem_op=1;
	sb.sem_flg=0;
	if(p == -1 || semop(p,&sb,1)==-1){
		perror(ERROR_GENERIC);
        _exit(EXIT_FAILURE);
	}
	//create nephew thread
	nipote1.semaphore=p;
	nipote2.semaphore=p;
	nipote1.figlio=pthread_self();
	nipote2.figlio=pthread_self();
	nipote1.id=1;
	nipote2.id=2;
	printf("Figlio: my thread id %ld", (long) pthread_self());
	pthread_t thread;
	int t=pthread_create(&thread,NULL,nipote_t,(void *) &nipote1);
	pthread_join(thread,NULL); //wait for a nephew
	send_terminate();
	//remove semaphore p
	int remove = semctl(p, 0, IPC_RMID, NULL);
	if (remove == -1){
		perror(ERROR_GENERIC);
		_exit(EXIT_FAILURE);
	}

	_exit(EXIT_SUCCESS);
}

void status_updated(){
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
