#include "../../include/thread/nipote_t.h"

void * nipote_t(void * pointer){
    struct Nipote me;
    me=*((struct Nipote *) pointer);
    int my_string;
	time_t start, end;
    struct timespec time_struct; //struct used to retrieve seconds
    time_t seconds;
    while(1){
        //acquire lock1
        if (pthread_mutex_lock(me.lock1) != 0){
        	perror(ERROR_GENERIC);
        	_exit(EXIT_FAILURE);
        }
        //CRITICAL SECTION
        //access status
        my_string=load_string();
        if(my_string==num_line_inputfile){
            S1--;
            //end nephew
            //release lock
            if (pthread_mutex_unlock(me.lock1) != 0){
		    	perror(ERROR_GENERIC);
		    	_exit(EXIT_FAILURE);
		    }
            pthread_exit(NULL); //end
        }else{
            //increment id_string
            (*S1)++;
            //set grandson
            (*(--S1))=me.id;
            //notify son
             if (pthread_kill(me.figlio,SIGUSR1) != 0){
		    	perror(ERROR_GENERIC);
		    	_exit(EXIT_FAILURE);
		    }
            if(clock_gettime(CLOCK_REALTIME,&time_struct)==-1){ //2038 year bug
                seconds=0;
            }
            seconds=time_struct.tv_sec;

			unsigned int key = find_key((char *) S1, num_line_inputfile - my_string, me.lock1);//unlock inside
			//END CRITICAL SECTION

            if (pthread_mutex_lock(me.lock2) != 0){
		    	perror(ERROR_GENERIC);
		    	_exit(EXIT_FAILURE);
		    }
			save_key(key, my_string);
			if (pthread_mutex_unlock(me.lock2) != 0){
		    	perror(ERROR_GENERIC);
		    	_exit(EXIT_FAILURE);
		    }

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
	if (pthread_mutex_unlock(lock) != 0){
    	perror(ERROR_GENERIC);
    	_exit(EXIT_FAILURE);
    }
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

void start_threads(unsigned int * plain_text, unsigned int * encoded_text, unsigned int * key){
    struct Thread args[NUM_THREAD_KEY];
    pthread_t threads[NUM_THREAD_KEY];
    //inizialize lock
    if (pthread_mutex_init(&end_thread,NULL) != 0){
    	perror(ERROR_GENERIC);
    	_exit(EXIT_FAILURE);
    }
    //acquire lock
    if (pthread_mutex_lock(&end_thread) != 0){
    	perror(ERROR_GENERIC);
    	_exit(EXIT_FAILURE);
    }
    unsigned int start=0;
    int i=0;
    int step=(UINT_MAX/NUM_THREAD_KEY);
    for(i; i<NUM_THREAD_KEY-1; i++){
        args[i].plain_text=plain_text;
        args[i].encoded_text=encoded_text;
        args[i].key=key;
        args[i].start=start;
        args[i].end=start+step;
        args[i].lock=&end_thread;
        start=start+step+1;
        if (pthread_create(&threads[i],NULL,find_key_thread,(void *) &(args[i])) != 0){
        	perror(ERROR_GENERIC);
        }
    }
    args[i].plain_text=plain_text;
    args[i].encoded_text=encoded_text;
    args[i].key=key;
    args[i].start=start;
    args[i].end=UINT_MAX;
    args[i].lock=&end_thread;
    if (pthread_create(&threads[i],NULL,find_key_thread,(void *) &(args[i])) != 0){
   		perror(ERROR_GENERIC);
   	}
    //now all thread are started
	if (pthread_mutex_lock(&end_thread) != 0){//this make it wait until the thread release lock
    	perror(ERROR_GENERIC);
    	_exit(EXIT_FAILURE);
    }
	
    //key is found, kill all the remaining thread
    for(i=0; i<NUM_THREAD_KEY; i++){
   		pthread_cancel(threads[i]);
    }
    
    int destroy_return = pthread_mutex_destroy(&end_thread);
    if (destroy_return != 0 && destroy_return != EBUSY){
    	perror(ERROR_GENERIC);
		_exit(EXIT_FAILURE);
	}
}

void * find_key_thread(void * pointer){
    struct Thread me = * ((struct Thread *) pointer);
    long int key;
    for (key = me.start; key <= me.end; key++){
		if ((*(me.plain_text) ^ key) == *(me.encoded_text)){
            *(me.key) = (unsigned int) key;
            //release semaphore
            if (pthread_mutex_unlock(me.lock) != 0){
				perror(ERROR_GENERIC);
				_exit(EXIT_FAILURE);
			}
			pthread_exit(NULL);
		}
	}
    pthread_exit(NULL);
}
