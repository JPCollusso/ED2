
#include "utils.h"

void clear_terminal(){
    
    #if defined(_WIN32) || defined (_WIN64)
        system("cls");
    #else 
        system("clear");
    #endif
}

void clean_stdin(){
    int c;
    while(((c = getchar()) != '\n') && (c != EOF));
}

void pause_terminal(){

    clean_stdin();
    getchar();
}


