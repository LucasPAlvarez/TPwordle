//dimenciones del grid
#define COLS 5
#define ROWS 5

//colores de texto
#define YELLOW "\033[1;43;30m"
#define GREEN "\033[1;42;30m"
#define DEFAULT "\033[0m"

//tamaño de la palabra en el juego (aunque sea 5 letras necesita el numero 7)
#define WORD_LENGTH 7

//comprueva si es mayuscula o no
#define ISLOWERC(a) ((('a' <= (a)) &&( (a) <= 'z'))?1:0)
//convierte minusculas en mayusculas
#define TOUPPER(a) ((a) = (char) ((a)-32))

//defines para el estado de comparacion de plalabras
#define LETTERCORRECT 2
#define LETTERINWORD 1
#define LETTERMISSING 0