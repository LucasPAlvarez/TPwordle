#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>

#ifndef  WORDLE_BASE
#define WORDLE_BASE
#include "parameters.h"
#include "wordleLogic.h"
#include "wordleVisuals.h"
#endif

char *wordList[5]; 
FILE *listaFile; 

//habre el archivo de palabras a adivinar del wordle
int openWordsFile (){
    if((listaFile = fopen("PALABAS-5LETRAS.txt", "r")) == NULL){
        printf("errno = %d\n", errno);
        return 0;
    }else{
        printf("archivo existe\n");
        return 1;
    }
}

//cierra el archivo de palabras a adivinar del wordle
void closeWordsFile (){
    fclose(listaFile);
}

int main(){
    int result = openWordsFile();
    if(result){
        closeWordsFile();
    }
}
