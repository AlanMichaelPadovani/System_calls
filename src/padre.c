#include "../include/padre.h"

int padre(char * input_path, char * output_path){
    int shmid=-1,fd_input=-1, fd_output=-1; //shmid of the shared memory, file descriptors
    int * S1; //location address

    //open the input file
    fd_input=open(input_path,O_RDONLY,S_IRUSR); //mode is ignored
    if(fd_input==-1){
        //error opening
        return 1;
    }
    //obtain the file size
    off_t file_size=lseek(fd_input,0,SEEK_END);
    if(file_size == -1){
        //error
        return 1;
    }
    //restore the offset on the start of file
    lseek(fd_input,0,SEEK_SET);

    S1=attach_segments(KEY_S1,file_size+sizeof(struct Status),&shmid);

    if((*S1) == -1){
        //error
        return 1;
    }
    //segment successfully obtained

    //transfer file into S1
    int num_line=load_file(fd_input,(char *) S1);

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
        printf("I'm logger\n");
        logger();
        return 0;
    }else{
        //father
        pid_t figlio;
        if((figlio=fork())==-1){
            //error creating son
            return 1;
        }
        if(figlio==0){
            //son
            printf("I'm son\n");
            return 0;
        }else{
            //father
            wait(NULL); //wait for logger
            wait(NULL); //wait for son
            printf("I'm father\n");
            //sleep(10);
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

int load_file(int fd, char * S1){
    char buffer[512];
    int r, index;
    char my_char='\n';
    int num_line=0;
    //loop for read the file
    while((r=read(fd,&buffer,512))>0){
        //loop for write
        for(index=0; index < r ; index++){
            if(buffer[index]==my_char) num_line++; //count line
            (*(S1++))=buffer[index]; //copy on shared memory
        }
    }
    return num_line;
}
void save_keys();
void check_keys();
