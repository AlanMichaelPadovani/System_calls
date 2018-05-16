#ifndef TYPES_H
#define TYPES_H

struct Status{
    int grandson;
    int id_string;
};

struct Message{
    long mtype;
    char text[128];
};

#endif
