#include "../include/nipote.h"

int nipote(int id, int sem, int * S1, int * S2, int num_line, int msgkey){
    int my_string;
    sb.sem_num=0;
	sb.sem_flg=0;
	time_t start, end;
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

			start = time(NULL);
			unsigned int key = find_key((char *) S1, num_line - my_string);
			save_key((char *) S2, key, my_string);
			sleep(2);
			end = time(NULL);
			send_timeelapsed(msgkey, end - start);

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
	char text[] = "chiave trovata in 0\n";
	text[18] = seconds + 48;
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

void save_key(char * S2,unsigned int key, int offset){
    //set S2 on the correct line
	S2 = S2 + (4 * offset);
    //cast S2 to unsigned int pointer
    unsigned int * p = (unsigned int *) S2;
    //set on S2 the key
    *p=key;
	return;

}
