#include<stdio.h>
int main(){
    int i, n, suma=0;
    for(i=1;i<=8;i++){
        printf("Ingrese un número entero: ");
        scanf("%d",&n);
        suma = suma + n;
    }
    printf("\nLa suma de todos los números ingresados es: %d\n",suma);
    return 0;
}
