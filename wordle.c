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

//------------------------- Variable -----------------------

//palabra a adivinar
char WordToGuess[WORD_LENGTH]; 
//palabra dada por el jugador
char WordPlayer[WORD_LENGTH];

// graba los espacios que fueron encontrados
int foundSpots[5];

//puntaje
int scoore;
//resultado de la comparacion entre palabras
int result[5];

//variables de grilla donde se guardan las letras y colores
char gridWordStorege[COLS][ROWS];
char *GridColorStorege[COLS][ROWS];
//archivo de palabras
FILE *listaFile;

//array para guardar todas las seciones que se juegan
struct Round Session[8];

//guardan el total de partidas y la partida que se esta jugando en este momento
int partActual, partTotal;

//--------------------- Manejo de file ----------------------------------

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
    do{
        int randPos = (rand()%30)* WORD_LENGTH;
        fseek(listaFile, randPos,SEEK_SET);
        fgets(WordToGuess, WORD_LENGTH, listaFile);
        WordToGuess[5] = '\0';
    }while(CheckUsedWords(WordToGuess));
}

//chequea si las palabras elejidas por la maquina al azar fueron usdas antes
int CheckUsedWords(char *palabra){
    int isUsed = 1;
    if(partActual > 1){
        for(int i = 0; i < partActual-1; i++){
            for(int j = 0; j<5; j++){
                if(Session[i].word[j] != palabra[j]){
                    isUsed = 0;
                    break;
                }
            }
            if(isUsed){
                //printf("La palabra %s, fue usada en la ronda %d\n", palabra, i+1);
                return 1;
            }else{
                //printf("la palabra %s, no fue usada en la ronda %d\n", palabra, i+1);
                isUsed = 1;
            }
        }
    }
    return 0;
}

//----------------------      --------------------------

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
//Inicializa el arreglo de resultados
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

//chequea si se an adivinado todas las letras
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

//calcula y actualiza el puntaje
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

// logica del input al pedir cantidad de partidas a jugar al jugador
int PedirCantPartidas(){
    scanf("%d",&partTotal);
    if(partTotal <= 8 && partTotal > 0){
        partActual = 1;
        return 1;
    }
    return 0;
}

//--------- manejadores de sesion -----------------------

// guarda los resultados de cada ronda en la sesion
void SaveRound(int roundResult){
    Session[partActual-1].nro = partActual;
    for(int i = 0; i < 5; i++){
       Session[partActual-1].word[i] = WordToGuess[i]; 
    }
    Session[partActual-1].word[5] = '\0';
    Session[partActual-1].scoore = scoore; 
    Session[partActual-1].wasWon = roundResult;
}

// imprime la ronda dada
void PrintRound(struct Round round){
    printf("Ronda: %d   Palabra a adivinar: %s   Puntaje: %d", round.nro, round.word, round.scoore);
    if(round.wasWon){
        printf("  La partida fue ganada.\n");
    }else{
        printf("  La partida fue perdida.\n");
    }
}

//imprime toda la sesion
void PrintSesion(){
    for(int i = 0; i < partActual; i++){
        PrintRound(Session[i]);
    }
}

//busca en la sesion la ronda con el puntaje mas alto y lo imprime
void PrintHighScoore(){
    int nroPartida = -1, highscoore = -1;
    for (int i = 0; i< partActual; i++){
        if(Session[i].scoore > highscoore){
            highscoore = Session[i].scoore;
            nroPartida = Session[i].nro;
        }
    }
    printf("En la partida %d tubo el mayor puntaje, con una puntuacion de %d\n", nroPartida, highscoore);
}

//busca en la sesion la ronda con el puntaje mas bajo y lo imprime
void PrintLowScoore(){
    int nroPartida = -1, lowscoore = 10001;
    for (int i = 0; i< partActual; i++){
        if(Session[i].scoore < lowscoore){
            lowscoore = Session[i].scoore;
            nroPartida = Session[i].nro;
        }
    }
    printf("En la partida %d tubo el mayor puntaje, con una puntuacion de %d\n", nroPartida, lowscoore);

}

// calcula e imprime el promedio de las rondas ganadas
void PrintAveregeScore(){
    int sumatemp = 0, cant = 0;
    for (int i = 0; i< partActual; i++){
        if(Session[i].wasWon){
            sumatemp += Session[i].scoore;
            cant++;
        }
    }
    if(cant != 0){
        printf("En la partida el promedio del puntaje de sus rondas ganadas fue %.2d\n", sumatemp/cant);
    }else{
        printf("No ah ganado ninguna partida\n");
    }
}

// -------------- funciones visuales -------------------

//imprime la palabra guardada
void printWord(){
    printf("%s\n", WordToGuess);
}

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

//menu inicial para pedir cuantas rondas desea jugar en esta sesion
void InitialMenu(){
    printf("Bienvenidos al juego de Wordle.\n\nCUantas partidas desea jugar (el maximo son 8):");
    while(1){
        if(PedirCantPartidas()){
            break;
        }
        printf("Debe ingresar unnumero entre 0 y 8\nIntentelo nuevamente:");
    }
}

//imprime la partida y ronda en la que esta el juego
void printHeadder(int turno){
    printf("Partida %d de %d            Turno %d\n\n", partActual, partTotal, turno);
}

//pregunta al jugador si desea seguir jugando o no
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
//imprime el puntaje en pantalla
void printScoore(){
    printf("Scoore: %d\n\n", scoore);
}

// --------------    game     -----------------

// maneja el juego
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
        if(again){
            partActual++;
        }
    }while(again);

    PrintSesion();
    PrintHighScoore();
    PrintLowScoore();
    PrintAveregeScore();

}

//maneja rondas individuales
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
            SaveRound(1);
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
        SaveRound(0);
    }
    printGrid();
    
    
}
