#ifndef PADRE_H
#define PADRE_H

#define KEY_S1 16
#define KEY_S2 17
#define MSG_KEY 18

#include <sys/shm.h>
#include <sys/ipc.h>
#include <stdio.h> //NULL
#include "types.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "logger.h"
#include "figlio.h"
//#include <stdlib.h> //sleep

int padre(char * input_path, char * output_path);
int * attach_segments(int key,int size, int * shmid);
int detach_segments(void * locate, int * shmid);
void load_file(int fd, char * S1, int * * save);
void save_keys();
void check_keys();

#endif
