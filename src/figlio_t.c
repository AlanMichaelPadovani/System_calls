#include "../include/figlio_t.h"

void figlio_t(int out){
	sem_out=out;
	sembuf_out.sem_num=0;
	sembuf_out.sem_flg=0;
	//status_updated​ signal handler of signal SIGUSR1
	signal(SIGUSR1, status_updated);

	//inizialize locks
	pthread_mutex_init(&lock1, NULL);
	pthread_mutex_init(&lock2, NULL);
	//create nephew thread
	nipote1.lock1=&lock1;
	nipote2.lock1=&lock1;
	nipote3.lock1=&lock1;
	nipote1.lock2=&lock2;
	nipote2.lock2=&lock2;
	nipote3.lock2=&lock2;
	nipote1.figlio=pthread_self();
	nipote2.figlio=pthread_self();
	nipote3.figlio=pthread_self();
	nipote1.id=1;
	nipote2.id=2;
	nipote3.id=3;
	pthread_t threads[3];
	int n1=pthread_create(&threads[0],NULL,nipote_t,(void *) &nipote1);
	int n2=pthread_create(&threads[1],NULL,nipote_t,(void *) &nipote2);
	int n3=pthread_create(&threads[2],NULL,nipote_t,(void *) &nipote3);
	pthread_join(threads[0],NULL); //wait for a nephew1
	pthread_join(threads[1],NULL); //wait for a nephew2
	pthread_join(threads[2],NULL); //wait for a nephew3
	//remove locks
	pthread_mutex_destroy(&lock1);
	pthread_mutex_destroy(&lock2);
	send_terminate();
	_exit(EXIT_SUCCESS);
}

void status_updated(){
	pthread_mutex_lock(&lock1);
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
	//acquire semaphore for stdout
	sembuf_out.sem_op=-1;
	if(semop(sem_out,&sembuf_out,1)==-1){
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
	sembuf_out.sem_op=1;
	if(semop(sem_out,&sembuf_out,1)==-1){
		//error releasing semaphore
		perror(ERROR_GENERIC);
		_exit(EXIT_FAILURE);
	}
	S1=temp; //restore S1
	pthread_mutex_unlock(&lock1);
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
