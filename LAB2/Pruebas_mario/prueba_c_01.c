#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define NUM_COLORS 4
#define MAX_ROUNDS 9

// Colores posibles
enum Color { ROJO, AMARILLO, AZUL, VERDE };

// Función para generar una combinación aleatoria de colores
void generar_combinacion(enum Color* combinacion, int longitud) {
    for (int i = 0; i < longitud; i++) {
        combinacion[i] = rand() % NUM_COLORS;
    }
}

// Función para mostrar la combinación de colores
void mostrar_combinacion(enum Color* combinacion, int longitud) {
    for (int i = 0; i < longitud; i++) {
        printf("%d ", combinacion[i]);
    }
    printf("\n");
}

// Función para leer la combinación del usuario
void leer_combinacion(enum Color* combinacion, int longitud) {
    for (int i = 0; i < longitud; i++) {
        int color;
        printf("Ingrese el color (0: Rojo, 1: Amarillo, 2: Azul, 3: Verde) para la posición %d: ", i + 1);
        scanf("%d", &color);
        if (color < 0 || color >= NUM_COLORS) {
            printf("Color inválido. Intente de nuevo.\n");
            i--; // Repetir la entrada para la posición actual
        } else {
            combinacion[i] = (enum Color)color;
        }
    }
}

// Función para comparar combinaciones
bool comparar_combinaciones(enum Color* comb1, enum Color* comb2, int longitud) {
    for (int i = 0; i < longitud; i++) {
        if (comb1[i] != comb2[i]) {
            return false;
        }
    }
    return true;
}

int main() {
    srand(time(NULL)); // Inicializar la semilla aleatoria
    
    enum Color combinacion_generada[MAX_ROUNDS];
    enum Color combinacion_usuario[MAX_ROUNDS];
    int longitud_combinacion = 4; // Longitud inicial de la combinación
    int estado = 0; // Estado de la máquina de estados
    int ronda = 1;

    // Inicializar la combinación generada con longitud inicial
    generar_combinacion(combinacion_generada, longitud_combinacion);

    while (true)
    {
        switch (estado)
    {
    case 0:
        printf("Esperando al usuario... (ingrese 1 para comenzar)\n");
        scanf("%d", &estado);
        break;
    
    // Primera combinacion
    case 1:
        generar_combinacion(combinacion_generada, longitud_combinacion);
        printf("Combinación generada: ");
        mostrar_combinacion(combinacion_generada, longitud_combinacion);
        printf("Ingrese su combinación:\n");
        leer_combinacion(combinacion_usuario, longitud_combinacion);

        if (comparar_combinaciones(combinacion_generada, combinacion_usuario, longitud_combinacion)) {
            printf("¡Correcto! Has acertado la combinación.\n");
            estado = estado + 1;
        } else {
            printf("Combinación incorrecta. Has perdido.\n");
            longitud_combinacion = 4; // Reiniciar la longitud de la combinación
            ronda = 1; // Reiniciar la ronda
            estado = 0; // Volver al estado de espera
        }
        break;
    
    case 2:
        // Agregar un nuevo color a la combinación generada
        combinacion_generada[longitud_combinacion] = rand() % NUM_COLORS;
        longitud_combinacion++; // Incrementar la longitud de la combinación
        ronda++; // Incrementar la ronda

        printf("Combinación generada: ");
        mostrar_combinacion(combinacion_generada, longitud_combinacion);
        printf("Ingrese su combinación:\n");
        leer_combinacion(combinacion_usuario, longitud_combinacion);

        if (comparar_combinaciones(combinacion_generada, combinacion_usuario, longitud_combinacion)) {
            printf("¡Correcto! Has acertado la combinación.\n");
            estado = estado + 1;
        } else {
            printf("Combinación incorrecta. Has perdido.\n");
            longitud_combinacion = 4; // Reiniciar la longitud de la combinación
            ronda = 1; // Reiniciar la ronda
            estado = 0; // Volver al estado de espera
        }
        break;
    
    case 3:
        // Agregar un nuevo color a la combinación generada
        combinacion_generada[longitud_combinacion] = rand() % NUM_COLORS;
        longitud_combinacion++; // Incrementar la longitud de la combinación
        ronda++; // Incrementar la ronda

        printf("Combinación generada: ");
        mostrar_combinacion(combinacion_generada, longitud_combinacion);
        printf("Ingrese su combinación:\n");
        leer_combinacion(combinacion_usuario, longitud_combinacion);
        
        if (comparar_combinaciones(combinacion_generada, combinacion_usuario, longitud_combinacion)) {
            printf("¡Correcto! Has acertado la combinación.\n");
            estado = estado + 1;
        } else {
            printf("Combinación incorrecta. Has perdido.\n");
            longitud_combinacion = 4; // Reiniciar la longitud de la combinación
            ronda = 1; // Reiniciar la ronda
            estado = 0; // Volver al estado de espera
        }
        break;
    
    case 4:
        // Agregar un nuevo color a la combinación generada
        combinacion_generada[longitud_combinacion] = rand() % NUM_COLORS;
        longitud_combinacion++; // Incrementar la longitud de la combinación
        ronda++; // Incrementar la ronda

        printf("Combinación generada: ");
        mostrar_combinacion(combinacion_generada, longitud_combinacion);
        printf("Ingrese su combinación:\n");
        leer_combinacion(combinacion_usuario, longitud_combinacion);
        
        if (comparar_combinaciones(combinacion_generada, combinacion_usuario, longitud_combinacion)) {
            printf("¡Correcto! Has acertado la combinación.\n");
            estado = estado + 1;
        } else {
            printf("Combinación incorrecta. Has perdido.\n");
            longitud_combinacion = 4; // Reiniciar la longitud de la combinación
            ronda = 1; // Reiniciar la ronda
            estado = 0; // Volver al estado de espera
        }
        break;
    
    case 5:
        // Agregar un nuevo color a la combinación generada
        combinacion_generada[longitud_combinacion] = rand() % NUM_COLORS;
        longitud_combinacion++; // Incrementar la longitud de la combinación
        ronda++; // Incrementar la ronda

        printf("Combinación generada: ");
        mostrar_combinacion(combinacion_generada, longitud_combinacion);
        printf("Ingrese su combinación:\n");
        leer_combinacion(combinacion_usuario, longitud_combinacion);
        
        if (comparar_combinaciones(combinacion_generada, combinacion_usuario, longitud_combinacion)) {
            printf("¡Correcto! Has acertado la combinación.\n");
            estado = estado + 1;
        } else {
            printf("Combinación incorrecta. Has perdido.\n");
            longitud_combinacion = 4; // Reiniciar la longitud de la combinación
            ronda = 1; // Reiniciar la ronda
            estado = 0; // Volver al estado de espera
        }
        break;
    
    case 6:
        // Agregar un nuevo color a la combinación generada
        combinacion_generada[longitud_combinacion] = rand() % NUM_COLORS;
        longitud_combinacion++; // Incrementar la longitud de la combinación
        ronda++; // Incrementar la ronda

        printf("Combinación generada: ");
        mostrar_combinacion(combinacion_generada, longitud_combinacion);
        printf("Ingrese su combinación:\n");
        leer_combinacion(combinacion_usuario, longitud_combinacion);
        
        if (comparar_combinaciones(combinacion_generada, combinacion_usuario, longitud_combinacion)) {
            printf("¡Correcto! Has acertado la combinación.\n");
            estado = estado + 1;
        } else {
            printf("Combinación incorrecta. Has perdido.\n");
            longitud_combinacion = 4; // Reiniciar la longitud de la combinación
            ronda = 1; // Reiniciar la ronda
            estado = 0; // Volver al estado de espera
        }
        break;

    case 7:
        // Agregar un nuevo color a la combinación generada
        combinacion_generada[longitud_combinacion] = rand() % NUM_COLORS;
        longitud_combinacion++; // Incrementar la longitud de la combinación
        ronda++; // Incrementar la ronda

        printf("Combinación generada: ");
        mostrar_combinacion(combinacion_generada, longitud_combinacion);
        printf("Ingrese su combinación:\n");
        leer_combinacion(combinacion_usuario, longitud_combinacion);
        
        if (comparar_combinaciones(combinacion_generada, combinacion_usuario, longitud_combinacion)) {
            printf("¡Correcto! Has acertado la combinación.\n");
            estado = estado + 1;
        } else {
            printf("Combinación incorrecta. Has perdido.\n");
            longitud_combinacion = 4; // Reiniciar la longitud de la combinación
            ronda = 1; // Reiniciar la ronda
            estado = 0; // Volver al estado de espera
        }
        break;
    
    case 8:
        // Agregar un nuevo color a la combinación generada
        combinacion_generada[longitud_combinacion] = rand() % NUM_COLORS;
        longitud_combinacion++; // Incrementar la longitud de la combinación
        ronda++; // Incrementar la ronda

        printf("Combinación generada: ");
        mostrar_combinacion(combinacion_generada, longitud_combinacion);
        printf("Ingrese su combinación:\n");
        leer_combinacion(combinacion_usuario, longitud_combinacion);
        
        if (comparar_combinaciones(combinacion_generada, combinacion_usuario, longitud_combinacion)) {
            printf("¡Correcto! Has acertado la combinación.\n");
            estado = estado + 1;
        } else {
            printf("Combinación incorrecta. Has perdido.\n");
            longitud_combinacion = 4; // Reiniciar la longitud de la combinación
            ronda = 1; // Reiniciar la ronda
            estado = 0; // Volver al estado de espera
        }
        break;
    
    case 9:
        // Agregar un nuevo color a la combinación generada
        combinacion_generada[longitud_combinacion] = rand() % NUM_COLORS;
        longitud_combinacion++; // Incrementar la longitud de la combinación
        ronda++; // Incrementar la ronda

        printf("Combinación generada: ");
        mostrar_combinacion(combinacion_generada, longitud_combinacion);
        printf("Ingrese su combinación:\n");
        leer_combinacion(combinacion_usuario, longitud_combinacion);
        
        if (comparar_combinaciones(combinacion_generada, combinacion_usuario, longitud_combinacion)) {
            printf("¡Correcto! Has acertado la combinación.\n");
            estado = estado + 1;
        } else {
            printf("Combinación incorrecta. Has perdido.\n");
            longitud_combinacion = 4; // Reiniciar la longitud de la combinación
            ronda = 1; // Reiniciar la ronda
            estado = 0; // Volver al estado de espera
        }
        break;
    
    default: estado = 0;
    
    }
    }
    
}
