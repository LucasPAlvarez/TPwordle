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

char GridWordStorege[COLS][ROWS];
char *GridColorStorege[COLS][ROWS];
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

//toma una palabra requiriendo que sea de 5 letras y combirtiendola en mayuscula
int getPlayerInput(){
    scanf( "%s", WordPlayer);
    printf("\n");
    if(!Is5Letters(WordPlayer)){
        return 0;
    }

    wordToUpper(WordPlayer);
    //printf("%s\n", WordPlayer);
    return 1;
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

//compara la palabra del jugador y guarda el resultado
int *CheckPlayerGuess(int turn){
    static int result[5] = {-1,-1,-1,-1,-1};
    
    for(int i = 0; i<5; i++){
        if(WordPlayer[i] == WordToGuess[i]){
            GridColorStorege[turn-1][i] = GREEN;
            GridWordStorege[turn-1][i] = WordPlayer[i];
            result[i] = LETTERCORRECT;
            continue;
        }
        for(int j = 0; j<5; j++){
            if(WordPlayer[i] == WordToGuess[j]){
                GridColorStorege[turn-1][i] = YELLOW;
                result[i] = LETTERINWORD;
                continue;
            }
        }
        if(result[i] == -1){
            result[i] = LETTERMISSING;
        }
        GridWordStorege[turn-1][i] = WordPlayer[i];
    }

    return result;
}

int checkResult(int *result){
    for(int i = 0; i<5;i++){
        if(result[i] == LETTERINWORD || result[i] == LETTERMISSING){
            return 0;
        }
    }
    return 1;
}

//inicializa los valores de las grillas a mostrar
void initializeGrid(){
    for (int i = 0; i < COLS; i++){
        for(int j = 0; j < ROWS; j++){
            GridWordStorege[i][j] = '*';
            GridColorStorege[i][j] = DEFAULT;
        }
    }
}

//imprime l grilla
void printGrid(){
    for (int i = 0; i < COLS; i++){
        for(int j = 0; j < ROWS; j++){
            printf("%s %c %s", GridColorStorege[i][j], GridWordStorege[i][j], DEFAULT);
        }
        printf("\n");
    }
}

//pide al jugaro que ingrese una palabra de 5 letras
void askForPlayerInput(){
    while(1){
        printf("Ingrese una palabra de 5 letras: ");
        if(getPlayerInput()){
            break;
        }
        printf("Error: Debe ingresar una palabra de 5 letras.\n");
    }
}

//limpia la pantalla
void ClearScreen(){
    system("clear");
}

//---------------------------------------------------

//borrar despues
void recordatorio(){
    if(1){
        printf("entra\n");
    }
}

// ------- game ----------
void playGame(){
    //initialize variables
    int turn = 1;
    int *result; 
    getRandWord();
    initializeGrid();
    
    do{
        //initialize visuals
        ClearScreen();
        printGrid();
        askForPlayerInput();

        //compare the input
        result = CheckPlayerGuess(turn);

        if(checkResult(result)){
            //print wining message and exit loop
            ClearScreen();
            printf("--- Felicidades ---\n\nAdivinaste la palabras.\n\n");
            break;
        }

        turn++;
    }while(turn <= 5);
    if(turn == 6){
        ClearScreen();
        //print losing message and show the word
        printf("--- Perdiste ---\n\nNo Adivinaste, la palabra era: %s\n\n",WordToGuess);
    }
    printGrid();
}
