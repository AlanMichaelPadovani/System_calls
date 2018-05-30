#include "../include/padre_t.h"

void main(int argc, char * argv[]){
    if(argc!=3){
        write(0, ERROR_INPUT_PARAMETERS, SIZE_ERROR_INPUT_PARAMETERS);
        _exit(EXIT_FAILURE);
    }
    padre(argv[1],argv[2]);
    _exit(EXIT_SUCCESS);
}
