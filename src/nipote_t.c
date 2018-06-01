#include "../include/nipote_t.h"

void * nipote_t(void * pointer){
    struct Nipote me;
    me=*((struct Nipote *) pointer);
    int my_string;
	time_t start, end;
    struct timespec time_struct; //struct used to retrieve seconds
    time_t seconds;
    while(1){
        //acquire lock1
        pthread_mutex_lock(me.lock1);
        //CRITICAL SECTION
        //access status
        my_string=load_string();
        if(my_string==num_line_inputfile){
            S1--;
            //end nephew
            //release lock
            pthread_mutex_unlock(me.lock1);
            pthread_exit(NULL); //end
        }else{
            //increment id_string
            (*S1)++;
            //set grandson
            (*(--S1))=me.id;
            //notify son
            pthread_kill(me.figlio,SIGUSR1);
            if(clock_gettime(CLOCK_REALTIME,&time_struct)==-1){ //2038 year bug
                seconds=0;
            }
            seconds=time_struct.tv_sec;

			unsigned int key = find_key((char *) S1, num_line_inputfile - my_string, me.lock1);//unlock inside
			//END CRITICAL SECTION

            pthread_mutex_lock(me.lock2);
			save_key(key, my_string);
            pthread_mutex_unlock(me.lock2);

            if(clock_gettime(CLOCK_REALTIME,&time_struct)==-1){ //2038 year bug
                seconds=0;
            }
            seconds=time_struct.tv_sec-seconds; //take second passed by difference
			send_timeelapsed(seconds);
        }
    }
    pthread_exit(NULL);
}

int load_string(){
    //S1 points at the next string
    return (*++S1);
}

unsigned int find_key(char * S1, int offset, pthread_mutex_t * lock){
    //STILL in CRITICAL SECTION
	S1 = S1 - (1030 * offset) + 1;

	char plain_text[4] = {*S1++, *S1++, *S1++, *S1};

	int text_size = 4;
	while ((*S1++) != ';') text_size++;

	S1 = S1 + 1;
	char encoded_text[4] = {*S1++, *S1++, *S1++, *S1};
	S1 = S1 + (1030 * offset) - text_size;

    //realease lock
	pthread_mutex_unlock(lock);
    //END of CRITICAL SECTION
	unsigned* plain_text_unsigned = (unsigned*) ((char *) plain_text);
	unsigned* encoded_text_unsigned = (unsigned*) ((char *) encoded_text);
    start_threads(plain_text_unsigned, encoded_text_unsigned, &key);
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
