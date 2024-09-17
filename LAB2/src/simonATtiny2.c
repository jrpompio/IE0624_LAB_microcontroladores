#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define standby 0                         // Define del estado de espera
#define secuence 1                        // Define del estado que genera la suecuencia
#define user 2                            // Define del estado que gestiona el reinicio del juego


int main(){
  
  srand(time(NULL));
  
  int state = standby;                    // Variable de estado
  int counterSecuence = 4;                // Variable para controlar el tamaño de la secuencia
  int start = 0;                          // Variable de arranque
  int secuenceLed[13] = {0};              // Array para guardar la secuencia
  int buttos[4] = {1,2,4,8};              // Numeros representativos de los botones
  int randNum;                            // Variable para guardar el nuevo color aleatorio


  while (1){
    switch (state){
      case standby:
        if (counterSecuence > 13){        // Si el tamaño de la secuencia supera 13 colores debe reiniciarse
          counterSecuence = 4;
        }
        if (counterSecuence == 4){
            printf("\n\nINICIANDO...\n");
            printf("\nDigite:\n");
        
            scanf("%d", &start);
            if (start != 0){                  // Al pulsar cualquier boton inicia el juego
                state = secuence;
                printf("%d",counterSecuence);

            // Ciclo for donde se genera la secuencia inicial
            for (int i = 0; i<= counterSecuence - 1; i++){
                randNum = buttos[rand() % 4];
                printf("\nnumero aleatorio es:\n %d", randNum);
                secuenceLed[i] = randNum;
            }
            } else {
                printf("esperando... \n");
            }
        // Aca se incrementa la secuencia por cada ronda correcta
        } else { 
            printf("\n\n ESTO SOLO DETIENE EL CODIGO BORRAR AL FINAL");
            scanf("%d", &start);
            randNum = buttos[rand() % 4];
            secuenceLed[counterSecuence] = randNum;
            for (int i = 0; i<= counterSecuence - 1; i++){
                printf("\nnumero aleatorio es:\n %d", secuenceLed[i]);
            }
            counterSecuence++;
        }
      break;
      case secuence:
        printf("\nanalizando secuencia\n");
        state = user;
        for (int i = 0; i<= counterSecuence -1; i++){
          printf("%d",secuenceLed[i]);
        }
        counterSecuence++;
      break;
      case user:
        state = standby;
        printf("\nestado que no sirve\n");
      break;
      default:
        printf("Caso default");
      break;
    }
  }
}
