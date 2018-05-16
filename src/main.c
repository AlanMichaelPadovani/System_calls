#include "../include/padre.h"
//#include "../include/logger.h"
#include "../include/figlio.h"
#include "../include/nipote.h"
#include "../include/types.h"

int main(int argc, char * argv[]){
    if(argc!=3){
        //error un parameters
        return 1;
    }
    padre(argv[1],argv[2]);
    return 0;
}
