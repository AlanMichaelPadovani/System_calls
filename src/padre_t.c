#include "../include/padre_t.h"

void padre(char * input_path, char * output_path){
    int shmid=-1,fd_input=-1, fd_output=-1; //shmid of the shared memory, file descriptors
    struct Memory save_info;
    int * * save= (int * *) get_space(&save_info,sizeof(int *));

    //open the input file
    fd_input=open(input_path,O_RDONLY,S_IRUSR); //mode is ignored
    if(fd_input==-1){
    	perror(ERROR_GENERIC);
        _exit(EXIT_FAILURE);
    }

    //obtain num_line_inputfile
    char buffer[512];
    int r, index;
	num_line_inputfile = 0;
    char my_char='\n';
    while((r=read(fd_input,&buffer,512))>0){
        for(index=0; index < r ; index++){
            if(buffer[index]==my_char) num_line_inputfile++; //count line
        }
    }

    S1=attach_segments(KEY_S1,(num_line_inputfile * 1030)+sizeof(struct Status),&shmid);
    int * S1_start = S1;
    if((*S1) == -1){
        perror(ERROR_GENERIC);
        _exit(EXIT_FAILURE);
    }
    //segment successfully obtained

    //transfer file into S1
    lseek(fd_input,0,SEEK_SET);
    load_file(fd_input,(char *) S1, save);
    close(fd_input); //close file input

    S1=*save; //set s1 to point at the next address after end of file in S1
    rem_space(&save_info);
    //create S2 in order to store the keys
    int shmid2=-1;
    S2=attach_segments(KEY_S2,num_line_inputfile*4,&shmid2); //each line contains a key of 32 bit
    pid_t logger_pid;
    if((logger_pid=fork())==-1){
        perror(ERROR_GENERIC);
        _exit(EXIT_FAILURE);
    }
    if(logger_pid==0){
        //logger
        logger();
    }else{
        //father
        pid_t figlio_pid;
        if((figlio_pid=fork())==-1){
            perror(ERROR_GENERIC);
       		_exit(EXIT_FAILURE);
        }
        if(figlio_pid==0){
            //son
			figlio_t();
        }else{
            //father
            wait(NULL); //wait for logger
            wait(NULL); //wait for son

            bool check = check_keys(num_line_inputfile, (char *) S1, (char *) S2);
			if (!check){
				//we have also to free memory
                //delete shared memory for S1
                if(detach_segments(S1_start,&shmid)==-1){
                    perror(ERROR_GENERIC);
        			_exit(EXIT_FAILURE);
                }
                //successfully removed shared space for S1

                //delete shared memory for S2
                if(detach_segments(S2,&shmid2)==-1){
                    perror(ERROR_GENERIC);
        			_exit(EXIT_FAILURE);
                }
                //successfully removed shared space for S2

                write(0, ERROR_CHECK_KEYS, SIZE_ERROR_CHECK_KEYS);
                _exit(EXIT_FAILURE);
			}

            save_keys(output_path,(unsigned int *) S2);
            //delete shared memory for S1
            if(detach_segments(S1_start,&shmid)==-1){
                perror(ERROR_GENERIC);
        		_exit(EXIT_FAILURE);
            }
            //successfully removed shared space for S1

            //delete shared memory for S2
            if(detach_segments(S2,&shmid2)==-1){
                perror(ERROR_GENERIC);
        		_exit(EXIT_FAILURE);
            }
            //successfully removed shared space for S2

            _exit(EXIT_SUCCESS);
        }
    }
}

int * attach_segments(int key, int size, int * shmid){

    //ask at the OS for the shared memory segment
    (*shmid)=shmget(key,size,IPC_CREAT|0666);
    //link this memory area to my address space
    if((*shmid) ==-1){
        perror(ERROR_GENERIC);
        _exit(EXIT_FAILURE);
    }
    return (int *) shmat((*shmid), NULL, 0);
}

int detach_segments(void * locate, int * shmid){
    //deatach this segment from my address space
    if(shmdt(locate)==-1){
        perror(ERROR_GENERIC);
        _exit(EXIT_FAILURE);
    }
    //eliminate this shared memory space
    return shmctl((*shmid),IPC_RMID,(struct shmid_ds *) NULL);
}

void load_file(int fd, char * S1, int * * save){
    char buffer[512];
    int r, index, c=0;
    char end_line='\n';
    //loop for read the file
    while((r=read(fd,&buffer,512))>0){
        //loop for write
        for(index=0; index < r ; index++){
            c++;
            if(buffer[index]==end_line){
                //endline
                (*(S1))=buffer[index]; //copy end line
                while(c<1030){
                    //is not max line size
                    S1++;
                    c++;
                }
                S1++;
                c=0;
            }else{
                (*(S1++))=buffer[index]; //copy on shared memory
            }
        }
    }
    int * p= (int * ) S1; //cast at int in order to perform correct increment of the address
    *save=p; //save this pointer value to update S1
    (*(++p))=0; //set string at 0
    (*(--p))=0; //set grandson
}

void save_keys(char * output_path, unsigned int * S2){
    //create or if exist truncate the file
    int fd=creat(output_path,O_WRONLY);
    //set readeable
    fchmod(fd,S_IRUSR|S_IWUSR);
    char prefix[]="0x";
    char key7, key6, key5, key4, key3, key2, key1, key0;
    int i;
    for(i=0; i< num_line_inputfile; i++, S2++){
        key7 = (((*S2 & 0xF0000000) >> 28) & 0x0000000F)+48;
        if(key7 > '9') key7=key7+7; //set HEX digit
        key6 = (((*S2 & 0x0F000000) >> 24) & 0x0000000F)+48;
        if(key6 > '9') key6=key6+7; //set HEX digit
        key5 = (((*S2 & 0x00F00000) >> 20) & 0x0000000F)+48;
        if(key5 > '9') key5=key5+7; //set HEX digit
        key4 = (((*S2 & 0x000F0000) >> 16) & 0x0000000F)+48;
        if(key4 > '9') key4=key4+7; //set HEX digit
        key3 = (((*S2 & 0x0000F000) >> 12) & 0x0000000F)+48;
        if(key3 > '9') key3=key3+7; //set HEX digit
        key2 = (((*S2 & 0x00000F00) >> 8) & 0x0000000F)+48;
        if(key2 > '9') key2=key2+7; //set HEX digit
        key1 = (((*S2 & 0x000000F0) >> 4) & 0x0000000F)+48;
        if(key1 > '9') key1=key1+7; //set HEX digit
        key0 = (*S2 & 0x0000000F)+48;
        if(key0 > '9') key0=key0+7; //set HEX digit
        write(fd,prefix,2);
        write(fd,&key7,1);
        write(fd,&key6,1);
        write(fd,&key5,1);
        write(fd,&key4,1);
        write(fd,&key3,1);
        write(fd,&key2,1);
        write(fd,&key1,1);
        write(fd,&key0,1);
        write(fd,"\n",1);
    }
    close(fd);
}

bool check_keys(int num_keys, char * S1, char * S2){
	bool check_result = true;
	char * app = S1;
    char * S1c; //pointer at cyphred text
    unsigned * key= (unsigned *) S2; //pointer at key
    unsigned * plain_text_unsigned;
    unsigned * encoded_text_unsigned;
    int i, j, k, str_len;
    char plain_text[4], encoded_text[4];
    //loop on lines
    for(i=num_keys; i>0 && check_result; i--){
		//restore S1 to start line
		S1=app-(i * 1030);
		S1c=S1+1029; //locate S1c on the end of line
		//locate end of line
		while(*S1c != '\n') S1c--;
		S1c= S1+(((--S1c) - S1) / 2); //make S1c point at the middle of the string
		str_len=(S1c-S1)-2;
        S1++; //skip <
        S1c=S1c+2; //skip <

        //main loop for check keys
        for(j=0; j<str_len; j=j+4){
            //inizialize plain_text and encoded_text
            for(k=0; k<4; k++) plain_text[k]=(*S1++);
            for(k=0; k<4; k++) encoded_text[k]=(*S1c++);
			plain_text_unsigned = (unsigned*) ((char *) plain_text);
			encoded_text_unsigned = (unsigned*) ((char *) encoded_text);
            if((*plain_text_unsigned ^ *key) != *encoded_text_unsigned){
                check_result = false;
				break;
            }
        }
		key++;
    }
	return check_result;
}
