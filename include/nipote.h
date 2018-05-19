#ifndef NIPOTE_H
#define NIPOTE_H

#include <sys/sem.h>
#include <stdio.h> //printf
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h> //sleep

struct sembuf sb;

int nipote(int id, int sem, int * S1, int num_line);
int load_string(int * S1);
int lock(int sem);
int unlock(int sem);
int find_key();
void send_timeelapsed();
void save_key();

#endif
