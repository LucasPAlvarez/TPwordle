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

// graba los espacios que fueron encontrados
int foundSpots[5];

int scoore;
int result[5];

char gridWordStorege[COLS][ROWS];
char *GridColorStorege[COLS][ROWS];
FILE *listaFile;

int partActual, partTotal;

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
//elimina los restos en caso de un stdin largo 
void garbegeCollector(){
    char *garbege;
    size_t len = 0;
    getline(&garbege, &len, stdin);
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

void resInit(){
    for(int i = 0; i<5; i++){
        result[i] = -1;
    }
}

//compara la palabra del jugador y guarda el resultado
int *CheckPlayerGuess(int turn){
    
    for(int i = 0; i<5; i++){
        if(WordPlayer[i] == WordToGuess[i]){
            GridColorStorege[turn-1][i] = GREEN;
            gridWordStorege[turn-1][i] = WordPlayer[i];
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
        gridWordStorege[turn-1][i] = WordPlayer[i];
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
            gridWordStorege[i][j] = '*';
            GridColorStorege[i][j] = DEFAULT;
        }
    }
}

//inicializa foundSpots
void FSstart(){
    for(int i = 0; i<5; i++){
        foundSpots[i] =1;
    }
}

//inicializa y resetea el scoore a 5000
void scooreStart(){
    scoore = 5000;
}

void calculateScore(int *result, int turn){
    if(checkResult(result)){
        if(turn == 1){
            scoore = 10000;
        }else{
            scoore += 2000;
        }

    }else{    
        
        //calculate new scoore
        //decrece por el intento usado
        scoore -= 500; 

        for(int i = 0; i<5; i++){
            scoore += (result[i] * 50) * foundSpots[i];
            if(result[i] == LETTERCORRECT){
                foundSpots[i] = 0;
            }
        }
    }
}

int PedirCantPartidas(){
    scanf("%d",&partTotal);
    if(partTotal <= 8 && partTotal > 0){
        partActual = 1;
        return 1;
    }
    return 0;
}

// -------------- funciones visuales -------------------

//imprime l grilla
void printGrid(){
    for (int i = 0; i < COLS; i++){
        for(int j = 0; j < ROWS; j++){
            printf("%s %c %s", GridColorStorege[i][j], gridWordStorege[i][j], DEFAULT);
        }
        printf("\n");
    }
}

//pide al jugaro que ingrese una palabra de 5 letras
void askForPlayerInput(){
    while(1){
        printf("\nIngrese una palabra de 5 letras: ");
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

void InitialMenu(){
    printf("Bienvenidos al juego de Wordle.\n\nCUantas partidas desea jugar (el maximo son 8):");
    while(1){
        if(PedirCantPartidas()){
            break;
        }
        printf("Debe ingresar unnumero entre 0 y 8\nIntentelo nuevamente:");
    }
}

void printHeadder(int turno){
    printf("Partida %d de %d            Turno %d\n\n", partActual, partTotal, turno);
}

int askContinuar(){
    while(1){
        char resp;
        printf("\nDesea continuar jugando[Y/N]: ");
        scanf("%c", &resp);
        if(resp == 'Y' || resp == 'y'){
            return 1;
        }
        if(resp == 'N' || resp == 'n'){
            return 0;
        }
        printf("\nDeve ingresar Y para si o N para no. Porfavor intentelo de nuevo");
    }
}

void printScoore(){
    printf("Scoore: %d\n\n", scoore);
}

// ------- game ----------
void playGame(){
    int again = 1;
    InitialMenu();
    do{
        playRound();
        if(partActual == partTotal){
            break;
        }
        garbegeCollector();
        again = askContinuar();
        partActual++;
    }while(again);

}


void playRound(){
    //initialize variables
    int turn = 1;
    getRandWord();
    initializeGrid();
    scooreStart();
    FSstart();
    resInit();
    do{
        //initialize visuals
        ClearScreen();
        printHeadder(turn);
        printScoore();
        printGrid();
        askForPlayerInput();

        //compare the input
        CheckPlayerGuess(turn);

        calculateScore(result, turn);

        if(checkResult(result)){
            //print wining message and exit loop
            ClearScreen();
            printHeadder(turn);
            printScoore();
            printf("--- Felicidades ---\n\nAdivinaste la palabra. Era ");
            printWord();
            printf("\n\n");
            break;
        }

        turn++;
    }while(turn <= 5);
    if(turn == 6){
        ClearScreen();
        //print losing message and show the word
        printHeadder(turn);
        printScoore();
        printf("--- Perdiste ---\n\nNo Adivinaste, la palabra era: %s\n\n",WordToGuess);
    }
    printGrid();
    
}
