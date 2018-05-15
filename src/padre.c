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

    S1=attach_segments(file_size,&shmid);

    if((*S1) == -1){
        //error
        return 1;
    }
    //segment successfully obtained
    //restore the offset on the start of file
    lseek(fd_input,0,SEEK_SET);
    int buffer[512];
    int r, index;
    int * S2=S1;
    //loop for read the file
    while((r=read(fd_input,&buffer,512))>0){
        //loop for write
        for(index=0; index < r ; index++){
            (*(S1++))=buffer[index];
        }
    }


    //use shared memory


    //delete shared memory
    if(detach_segments(S1,&shmid)==-1){
        //error
        return 1;
    }
    //successfully removed shared space
    return 0;
}

int * attach_segments(int size, int * shmid){
    //add also the size of the Status struct
    size=size+sizeof(struct Status);
    //ask at the OS for the shared memory segment
    (*shmid)=shmget(KEY,size,IPC_CREAT|0666);
    //link this memory area to my address space
    if((*shmid) ==-1){
        //cannot obtain shared memory
        //return NULL;
    }
    return (int *) shmat((*shmid), NULL, 0);
};

int detach_segments(void * locate, int * shmid){
    //deatach this segment from my address space
    if(shmdt(locate)==-1){
        //errors
        return -1;
    }
    //eliminate this shared memory space
    return shmctl((*shmid),IPC_RMID,(struct shmid_ds *) NULL);
};
void load_file();
void save_keys();
void check_keys();
