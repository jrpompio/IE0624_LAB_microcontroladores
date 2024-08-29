#include <stdio.h>

int main(){
    int valor = 95;
    printf("%b \n",valor); // Mostrando valor binario
    int decenas = valor / 10; 
    int unidades = valor % 10;
    decenas = (decenas << 4);
    valor = decenas | unidades;
    printf("%b \n",valor); // Mostrando valor binario
};