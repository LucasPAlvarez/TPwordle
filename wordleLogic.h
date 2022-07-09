//struct para guardar los datos de cada ronda
struct Round
{
    int nro;
    char word[7];
    int scoore;
    int wasWon;
};

//funciones
int openWordsFile ();
void closeWordsFile ();
void getRandWord();
int CheckUsedWords(char *palabra);
void printWord();
void garbegeCollector();
int getPlayerInput();
int Is5Letters(char *palabra);
void wordToUpper(char *palabra);
void resInit();
int *CheckPlayerGuess(int turn);
int checkResult(int *result);
void initializeGrid();
void FSstart();
void scooreStart();
void calculateScore(int *result, int turn);
int PedirCantPartidas();
void SaveRound(int roundResult);
void PrintRound(struct Round round);
void PrintSesion();
void PrintHighScoore();
void PrintLowScoore();
void PrintAveregeScore();
void playRound();
void playGame();