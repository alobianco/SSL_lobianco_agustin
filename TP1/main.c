#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


char* tipoDeConstante(char* palabra)
{

  int transiciones[7][6] = {
        /*     [0] [1-7] [8-9] [a-f] [x] [*] */
    /* q0-*/  { 1,   2,     2,    6,  6,  6},
    /* q1+ */ { 5,   5,     6,    6,  3,  6},
    /* q2+ */ { 2,   2,     2,    6,  6,  6},
    /* q3 */  { 4,   4,     4,    4,  6 , 6},
    /* q4+ */ { 4,   4,     4,    4,  6 , 6},
    /* q5+ */ { 5,   5,     6,    6,  6,  6},
    /* q6 */  { 6,   6,     6,    6,  6,  6}

  };
  char intervalos[5][2] = {
    {'0', '0'},
    {'1', '7'},
    {'8', '9'},
    {'a', 'f'},
    {'x', 'x'},
  };

  int estadoActual = 0;
  char caracter;
  int length = strlen(palabra);

  for(int i = 0; i < length; i++) {
    caracter = tolower(palabra[i]);

    int siguienteColumna = -1;

    for (int j = 0; j < 5; j++) {
      if (caracter >= intervalos[j][0] && caracter <= intervalos[j][1]) {
        siguienteColumna = j;
        break;
      }
    }

    if (siguienteColumna == -1) {
      siguienteColumna = 5;
    }

    estadoActual = transiciones[estadoActual][siguienteColumna];

  }



  switch(estadoActual) {
    case 1:
      return "OCTAL";
    case 2: 
      return "DECIMAL";
    case 4: 
      return "HEXADECIMAL";
    case 5:
      return "OCTAL";
    default:
      return "NO RECONOCIDA";
  }

}


int main() {
    FILE *fpLectura;
    FILE *fpEscritura;
    char *linea = NULL; //Es NULL porque getline lo va a aumentar o achicar como sea necesario
    char *hastaToken = NULL; 
    char *token = ","; //Creamos un char* que contiene el token que vamos a usar de limitante, en este caso la , 
    char *endptr; // Para el strtol
    size_t largo = 0; //Es 0 porque no es necesario que tenga tamaño, esto se cambia solo con el getline, son size_t porque es la variable que tiene getline por defecto
    size_t read; //Es un size_t porque es la variable que devuelve el getline

    fpLectura = fopen("entrada.txt", "r"); 
    fpEscritura = fopen("salida.txt", "w"); 
    if (fpLectura == NULL) {
        printf("Archivo vacio\n"); //Aviso que el archivo esta vacio
        exit(EXIT_FAILURE); 
    } 

    while ((read = getline(&linea, &largo, fpLectura)) != -1) { //getline, toma todas las palabras de una linea hasta el \n, tambien funciona como un malloc y un realloc, devuelve -1 si encuentra un \0 o un error que termine el texto
        hastaToken = strtok(linea, token); //El strtok, agarra la linea que encuentra getline (hasta el \n) y lo devuelve cuando encuentra el primer tok en, en este caso la coma ",", devuelve NULL cuando termina el string que estamos buscando
        while (hastaToken != NULL) { 
           
           fputs(hastaToken, fpEscritura);

           // Tabulacion hasta 20 caracters
           for (int i = strlen(hastaToken); i < 20; i++) {
            fputc(' ',fpEscritura);
           }

           fputs(tipoDeConstante(hastaToken), fpEscritura);
           fputc('\n',fpEscritura);
           
            hastaToken = strtok(NULL, token); //Se pasa NULL porque queremos que siga usando el mismo string que le pasamos antes
        }
    }


    fclose(fpLectura); 
    fclose(fpEscritura); 
    free(linea); 
    exit(EXIT_SUCCESS); 

}

