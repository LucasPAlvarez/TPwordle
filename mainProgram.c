//funciones del juego
#include "wordle.c"

int main(){
    ClearScreen();
    int result = openWordsFile();
    if(result){
        playGame();

        //end of program
        closeWordsFile();
    }

    return 0;
}