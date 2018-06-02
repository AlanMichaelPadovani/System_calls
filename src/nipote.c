#include "../include/nipote.h"

void nipote(int id){
    int my_string;
	sb.sem_flg=0;
	time_t start, end;
    struct timespec time_struct; //struct used to retrieve seconds
    time_t seconds;
    //acquire semaphore
    while(1){
        lock(sem_mem,0);
        //CRITICAL SECTION
        //access status
        my_string=load_string();
        if(my_string==num_line_inputfile){
            S1--;
            //end nephew
            unlock(sem_mem,0);
            _exit(EXIT_SUCCESS);
        }else{
            //increment id_string
            (*S1)++;
            //set grandson
            (*(--S1))=id;
            //notify son
            kill(getppid(),SIGUSR1);
            if(clock_gettime(CLOCK_REALTIME,&time_struct)==-1){ //2038 year bug
                seconds=0;
            }
            seconds=time_struct.tv_sec;

			unsigned int key = find_key((char *) S1, num_line_inputfile - my_string);//unlock sem_mem inside
			//END CRITICAL SECTION

			lock(sem_mem,1);
			save_key(key, my_string);
			unlock(sem_mem,1);

            if(clock_gettime(CLOCK_REALTIME,&time_struct)==-1){ //2038 year bug
                seconds=0;
            }
            seconds=time_struct.tv_sec-seconds; //take second passed by difference
			send_timeelapsed(seconds);
        }
    }
}

int load_string(){
    //S1 points at the next string
    return (*++S1);
}

void lock(int sem, int num_sem){
    lock_semaphore(sem, num_sem);
}

void unlock(int sem, int num_sem){
	unlock_semaphore(sem, num_sem);
}

unsigned int find_key(char * S1, int offset){

	S1 = S1 - (1030 * offset) + 1;

	char plain_text[4] = {*S1++, *S1++, *S1++, *S1};

	int text_size = 4;
	while ((*S1++) != ';') text_size++;

	S1 = S1 + 1;
	char encoded_text[4] = {*S1++, *S1++, *S1++, *S1};
	S1 = S1 + (1030 * offset) - text_size;

	unlock(sem_mem,0);

	unsigned* plain_text_unsigned = (unsigned*) ((char *) plain_text);
	unsigned* encoded_text_unsigned = (unsigned*) ((char *) encoded_text);

	long int key;
	for (key = 0; key <= UINT_MAX; key++){
		if ((*plain_text_unsigned ^ key) == *encoded_text_unsigned){
			break;
		}
	}
	return key;
}

void send_timeelapsed(int seconds){
	int msgid;
	if((msgid=msgget(MSG_KEY,0666)) == -1){
		perror(ERROR_GENERIC);
        _exit(EXIT_FAILURE);
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
		perror(ERROR_GENERIC);
        _exit(EXIT_FAILURE);
	}
}

void save_key(unsigned int key, int offset){
    //set S2 on the correct line
	char * S2c = (char *) S2 + (4 * offset);
    //cast S2 to unsigned int pointer
    unsigned int * p = (unsigned int *) S2c;
    //set on S2 the key
    *p=key;
}
