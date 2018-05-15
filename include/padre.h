#ifndef PADRE_H
#define PADRE_H

#define KEY 10
#include <sys/shm.h>
#include <sys/ipc.h>
#include <stdio.h> //NULL
#include "types.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
//#include <stdlib.h> //sleep

int padre(char * input_path, char * output_path);
int * attach_segments(int size, int * shmid);
int detach_segments(void * locate, int * shmid);
void load_file();
void save_keys();
void check_keys();

#endif
