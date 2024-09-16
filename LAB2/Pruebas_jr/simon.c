#include <stdio.h>
#include <stdlib.h>
#include <time.h>

enum event {
  standby,
  secuence,
  user
};


int main(){
  
  srand(time(NULL));
  
  enum event state = standby;
  int counterLed = 4;
  int start = 0;
  int secLed[13] = {0};
  int buttos[4] = {1,2,4,8};
  int randNum;


  while (1){
    switch (state){
      case standby:
        if (counterLed > 13){
          counterLed = 4;
        }
        printf("\n\nINICIANDO...\n");
        printf("\nDigite:\n");
        
        scanf("%d", &start);
        if (start != 0){
          state = secuence;
          printf("%d",counterLed);
          for (int i = 0; i<= counterLed - 1; i++){
            randNum = buttos[rand() % 4];
            printf("\nnumero aleatorio es:\n %d", randNum);
            secLed[i] = randNum;
          }
        } else {
          printf("esperando... \n");
        }
      break;
      case secuence:
        printf("\nanalizando secuencia\n");
        state = user;
        for (int i = 0; i<= counterLed -1; i++){
          printf("%d",secLed[i]);
        }
        counterLed++;
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
