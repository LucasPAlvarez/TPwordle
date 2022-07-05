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

int *CheckPlayerGuess(char *guess, char *hiddenWord){
    static int result[5] = {-1,-1,-1,-1,-1};
    
    for(int i = 0; i<5; i++){
        if(guess[i] == hiddenWord[i]){
            result[i] = LETTERCORRECT;
            continue;
        }
        for(int j = 0; j<5; j++){
            if(guess[i] == hiddenWord[j]){
                result[i] = LETTERINWORD;
                continue;
            }
        }
        if(result[i] == -1){
            result[i] = LETTERMISSING;
        }
    }

    return result;
}

//borrar despues
void recordatorio(){
    if(1){
        printf("entra\n");
    }
}

void checkTest(){
    getRandWord();
    printWord();
    getPlayerInput();
    int* result = CheckPlayerGuess(WordPlayer, WordToGuess);
    for(int i = 0; i<5; i++){
        printf("La letra a adivinar es: %c  y adivino: %c. Elresultado fue: %d\n", WordToGuess[i], WordPlayer[i], result[i]);
    }
}

int main(){
    int result = openWordsFile();
    if(result){
        checkTest();
        //getPlayerInput();
        //getRandWord();
        //printWord();
        closeWordsFile();
    }
}
