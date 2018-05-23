#include "../include/nipote.h"

int nipote(int id, int sem, int * S1, int * S2, int num_line, int msgkey){
    int my_string;
    sb.sem_num=0;
	sb.sem_flg=0;
	time_t start, end;
    struct timespec time_struct; //struct used to retrieve seconds
    time_t seconds;
    //acquire semaphore
    while(1){
        lock(sem);
        //CRITICAL SECTION
        //access status
        my_string=load_string(++S1);
        if(my_string==num_line){
            S1--;
            //end nephew
            unlock(sem);
            return 0;
        }else{
            //increment id_string
            (*S1)++;
            //set grandson
            (*(--S1))=id;
            //notify son
            kill(getppid(),SIGUSR1);
            if(clock_gettime(CLOCK_REALTIME,&time_struct)==-1){ //2038 year bug
                printf("Error getting start time");
                seconds=0;
            }
            seconds=time_struct.tv_sec;
			//start = time(NULL);
			unsigned int key = find_key((char *) S1, num_line - my_string);
			save_key((char *) S2, key, my_string);
			//sleep(2);
			//end = time(NULL);
            if(clock_gettime(CLOCK_REALTIME,&time_struct)==-1){ //2038 year bug
                printf("Error getting end");
                seconds=0;
            }
            seconds=time_struct.tv_sec-seconds; //take second passed by difference
			send_timeelapsed(msgkey, seconds);

			sleep(1);
            unlock(sem);
			//END CRITICAL SECTION
        }
    }
}

int load_string(int * S1){
    //S1 points at the next string
    return (*S1);
}

int lock(int sem){
    //acquire the semaphore
	sb.sem_op=-1;
	if(semop(sem,&sb,1)==-1){
		//error acquiring semaphore
		return 1;
	}
    return 0;
}

int unlock(int sem){
    //release the semaphore
    sb.sem_op=1;
    if(semop(sem,&sb,1)==-1){
        //error acquiring semaphore
        return 1;
    }
    return 0;
}

unsigned int find_key(char * S1, int offset){

	S1 = S1 - (1030 * offset) + 1;
	char plain_text[4] = {*S1++, *S1++, *S1++, *S1};

	int text_size = 4;
	while ((*S1++) != ';') text_size++;

	S1 = S1 + 1;
	char encoded_text[4] = {*S1++, *S1++, *S1++, *S1};
	S1 = S1 + (1030 * offset) - text_size;

    //printf("plain: %s, crypt: %s\n", plain_text, encoded_text);

	unsigned* plain_text_unsigned = (unsigned*) ((char *) plain_text);
	unsigned* encoded_text_unsigned = (unsigned*) ((char *) encoded_text);

	unsigned int key;
	for (key = 0; key < UINT_MAX; key++){
		if ((*plain_text_unsigned ^ key) == *encoded_text_unsigned){
			//printf("key: %x, %d\n", key, key);
			break;
		}
	}

	return key;
}

void send_timeelapsed(int msgkey, int seconds){
	int msgid;
	if((msgid=msgget(msgkey,0666)) == -1){
		//can't get the message queue
		return;
	}
	struct Message msg;
	msg.mtype=2;
	char text[] = "chiave trovata in ";

    //transform seconds into char
    int power =1,cifre=1;
    for(cifre;(power*10)<=seconds;cifre++, power=power*10); //obtain number of digits
    int num_cifre=cifre;
    char number[num_cifre]; //transform into char
    while(cifre!=0){
        number[num_cifre-cifre]=seconds%10;
        seconds=seconds/10;
        cifre--;
    }
    int index=0;
    char * pointer=&text[0];
	while((*pointer)!='\0'){
		msg.text[index++]=(*(pointer++)); //insert message text part
	}
    while((--num_cifre)!=-1){ //control overflow
        msg.text[index++]=number[num_cifre] +48; //insert message numeric part
    }
    msg.text[index++]='\n';
	msg.text[index]='\0';

	if(msgsnd(msgid,&msg,sizeof(msg)-sizeof(long),0)==-1){
		//error sending terminate message
		return;
	}
	return;
}

void save_key(char * S2,unsigned int key, int offset){
    //set S2 on the correct line
	S2 = S2 + (4 * offset);
    //cast S2 to unsigned int pointer
    unsigned int * p = (unsigned int *) S2;
    //set on S2 the key
    *p=key;
	return;

}
