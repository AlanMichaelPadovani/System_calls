#ifndef PADRE_H
#define PADRE_H

#include "utility.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include "logger.h"
#include "figlio.h"

void padre(char * input_path, char * output_path);
int * attach_segments(int key, int size, int * shmid);
int detach_segments(void * locate, int * shmid);
void load_file(int fd, char * S1, int * * save);
void save_keys(char * output_path, unsigned int * S2);
bool check_keys(int num_keys, char * S1, char * S2);

#endif
