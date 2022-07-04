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
char WordPlayer[WORD_LENGTH];
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

//pide una palabra al jugador requiriendo que sea de5 letras y combirtiendola en mayuscula
void getPlayerInput(){
    while(1){
        printf("Ingrese una palabra de 5 letras: ");
        scanf( "%s", WordPlayer);
        printf("\n");
        if(Is5Letters(WordPlayer)){
            break;
        }else{
            printf("Error: Debe ingresar una palabra de 5 letras.\n");
        }
    }

    wordToUpper(WordPlayer);
    printf("%s\n", WordPlayer);
}

//cuenta cunatas letras tiene la palabra y verifica si es igual a 5
int Is5Letters(char *palabra){
    int counter;
    for (counter = 0; palabra[counter] != '\0'; ++counter);
    return (counter == 5);
}

// transforma todas las minusculas en mayusculas
void wordToUpper(char *palabra){
    for(int i = 0; palabra[i] != '\0'; i++){
        if(ISLOWERC(palabra[i])){
            TOUPPER(palabra[i]);
        }
    }
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
        //getPlayerInput();
        //getRandWord();
        //printWord();
        closeWordsFile();
    }
}
