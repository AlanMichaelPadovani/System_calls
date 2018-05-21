#include "../include/padre.h"

int padre(char * input_path, char * output_path){
    int shmid=-1,fd_input=-1, fd_output=-1; //shmid of the shared memory, file descriptors
    int * S1; //location address
    //int * * save=malloc(sizeof(int *));
    struct Memory save_info;
    int * * save= (int * *) get_space(&save_info,sizeof(int *));
    //open the input file
    fd_input=open(input_path,O_RDONLY,S_IRUSR); //mode is ignored
    if(fd_input==-1){
        //error opening
        return 1;
    }
    //obtain num_line
    char buffer[512];
    int num_line=0, r, index;
    char my_char='\n';
    while((r=read(fd_input,&buffer,512))>0){
        for(index=0; index < r ; index++){
            if(buffer[index]==my_char) num_line++; //count line
        }
    }

    /*
    off_t file_size=lseek(fd_input,0,SEEK_END);
    if(file_size == -1){
        //error
        return 1;
    }
    //restore the offset on the start of file
    lseek(fd_input,0,SEEK_SET);
    */

    S1=attach_segments(KEY_S1,(num_line * 1030)+sizeof(struct Status),&shmid);

    if((*S1) == -1){
        //error
        return 1;
    }

    //segment successfully obtained

    //transfer file into S1
    lseek(fd_input,0,SEEK_SET);
    load_file(fd_input,(char *) S1, save);
    close(fd_input); //close file input

    S1=*save; //set s1 to point at the next address after end of file in S1
    //create S2 in order to store the keys
    int shmid2=-1;
    int * S2=attach_segments(KEY_S2,num_line*4,&shmid2); //each line contains a key of 32 bit
    pid_t logger_pid;
    if((logger_pid=fork())==-1){
        //error creating logger
        return 1;
    }
    if(logger_pid==0){
        //logger
        logger(MSG_KEY);
    }else{
        //father
        pid_t figlio_pid;
        if((figlio_pid=fork())==-1){
            //error creating son
            return 1;
        }
        if(figlio_pid==0){
            //son
			figlio(S1, S2, MSG_KEY, num_line);
        }else{
            //father
            wait(NULL); //wait for logger
            wait(NULL); //wait for son

            bool check = check_keys(num_line,(char *) S1,(char *) S2);
			if (!check){
				return 1;
			}
			printf("save_keys...\n");
					
            //free(save);
            rem_space(&save_info);
            //delete shared memory for S1
            if(detach_segments(S1,&shmid)==-1){
                //error
                return 1;
            }
            //successfully removed shared space for S1

            //delete shared memory for S2
            if(detach_segments(S2,&shmid2)==-1){
                //error
                return 1;
            }
            //successfully removed shared space for S2
            return 0;
        }
    }
}

int * attach_segments(int key, int size, int * shmid){

    //ask at the OS for the shared memory segment
    (*shmid)=shmget(key,size,IPC_CREAT|0666);
    //link this memory area to my address space
    if((*shmid) ==-1){
        //cannot obtain shared memory
        //return NULL;
    }
    return (int *) shmat((*shmid), NULL, 0);
}

int detach_segments(void * locate, int * shmid){
    //deatach this segment from my address space
    if(shmdt(locate)==-1){
        //errors
        return -1;
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
    return;
}
void save_keys();
bool check_keys(int num_keys, char * S1, char * S2){
    
	bool check_result = true;
	char * app = S1;  
    unsigned * key= (unsigned *) S2;
	int i, j;
    for(i=num_keys; i>0 && check_result; i--){
		//restore S1 to start line
		S1=app-(i * 1030);
		char * S1c=S1+1029; //locate S1c on the end of line
		//locate end of line
		while(*S1c != '\n') S1c--;
		S1c= S1+(((S1c) - S1) / 2) - 1; //make S1c point at the middle of the string
		int str_len=(S1c-S1)-2;

        //main loop for check keys
        S1++; //skip <
        S1c=S1c+2; //skip <

        for(j=0; j<str_len; j=j+4){
			char plain_text[4] = {*S1++, *S1++, *S1++, *S1++};
			char encoded_text[4] = {*S1c++, *S1c++, *S1c++, *S1c++};
			unsigned* plain_text_unsigned = (unsigned*) ((char *) plain_text);
			unsigned* encoded_text_unsigned = (unsigned*) ((char *) encoded_text);
            if((*plain_text_unsigned ^ *key) != *encoded_text_unsigned){
                check_result = false;
				break;
            }
        }
		key++;
    }
	return check_result;
}
