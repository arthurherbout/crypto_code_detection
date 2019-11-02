#include "stdio.h"

using namespace std;

int main(){
  int c,i,j,k,MA[105][105],NA[105][105],f,m,n;

  scanf("%d", &c);
  for(i=1; i<=c; i++){
    scanf("%d %d", &m, &n);
    for(j=1; j<=m; j++){
      for(k=1; k<=n; k++){
        scanf("%d", &MA[j][k]);
      }
    }

    // Inicializar el pasto como SIN CORTAR
    for(j=1; j<=n; j++){
      for(k=1; k<=m; k++){
        NA[k][j] = 2;
      }
    }

    // Verifica por columnas iniciales
    for(j=1; j<=n; j++){
      if(MA[1][j] == 1){
        f = 1;
        // Verifica si el camino esta libre
        for(k=1; k<=m; k++){
          if(MA[k][j] == 2){
            f = 0;
            k = m;
          }
        }
        // Si esta libre, cortar
        if(f){
          for(k=1; k<=m; k++){
            NA[k][j] = 1;
          }
        }
      }
    }

    // Verifica por filas iniciales
    for(j=1; j<=m; j++){
      if(MA[j][1] == 1){
        f = 1;
        // Verifica si el camino esta libre
        for(k=1; k<=n; k++){
          if(MA[j][k] == 2){
            f = 0;
            k = n;
          }
        }
        // Si esta libre, cortar
        if(f){
          for(k=1; k<=n; k++){
            NA[j][k] = 1;
          }
        }
      }
    }

    // Comparar matrices y determinar resultado
    f = 1;
    for(j=1; j<=m; j++){
      for(k=1; k<=n; k++){
        if(MA[j][k] != NA[j][k]){
          f = 0;
          j = m;
          k = n;
        }
      }
    }

    if(f) printf("Case #%d: YES\n", i);
    else printf("Case #%d: NO\n", i);
  }
}