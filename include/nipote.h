#ifndef NIPOTE_H
#define NIPOTE_H

#include <sys/sem.h>
#include <stdio.h> //printf

struct sembuf sb;

int nipote(int sem, int * S1);
int load_string(int * S1);
int lock(int sem);
int unlock(int sem);
int find_key();
void send_timeelapsed();
void save_key();

#endif
