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

char WordToGuess[WORD_LENGTH]; 
FILE *listaFile; 

//habre el archivo de palabras a adivinar del wordle
int openWordsFile (){
    if((listaFile = fopen("PALABAS-5LETRAS.txt", "rb")) == NULL){
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

// consigue una palabra al azar de la lista de palabras
void getRandWord(){
    srand(time(NULL));
    int randPos = (rand()%30)* WORD_LENGTH;
    fseek(listaFile, randPos,SEEK_SET);
    fgets(WordToGuess, WORD_LENGTH, listaFile);
}

//imprime la palabra guardada
void printWord(){
    printf("%s\n", WordToGuess);
}

//borrar despues
void recordatorio(){
    if(1){
        printf("entra\n");
    }
}

//prueva para leer el archivo de palabras, borrar antes de terminar el tp
void pruevaLectura (){
    char palabra[7]; //= (char *)malloc(sizeof(char)*10);
    
    //fgets(palabra, 10, listaFile);
    //printf("%d \n" , feof(listaFile));
    /*while(! feof(listaFile)){
        //printf("entra\n");
        //fread(palabra, sizeof(char), 7, listaFile);
        fgets(palabra, 10, listaFile);
        printf("%s - ", palabra);
        printf("Position: %ld\n", ftell(listaFile));
    }
    printf("exit 88888\n");*/

    //rewind(listaFile);
    fseek(listaFile, 203, SEEK_SET);
    fgets(palabra, 10, listaFile);
    for (int i = 0; i < 5; i++){
        printf("%c -\n", (char)palabra[i]);
    }
    
    //free(palabra);
    
}
int main(){
    int result = openWordsFile();
    if(result){
        //recordatorio();
        //pruevaLectura();
        getRandWord();
        printWord();
        closeWordsFile();
    }
}
