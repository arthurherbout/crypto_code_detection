#include "stdio.h"
using namespace std;

char gana(char a, char b, char c, char d){
  if(a=='T' && b==c && b==d){
    return b;
  }else if(d=='T' && a==b && a==c){
    return a;
  }else if(a==b && a==c && a==d){
    return a;
  }
  return '.';
}

int printGana(char a, int c){
  if(a != '.'){
    printf("Case #%d: %c won\n", c, a);
    return 1;
  }
  return 0;
}

int main(){
  int n,i,j,k,fFinalizado;
  char M[5][5],r;
  scanf("%d", &n);
  for(i=1; i<=n; i++){
    scanf("%s", M[0]);
    scanf("%s", M[1]);
    scanf("%s", M[2]);
    scanf("%s", M[3]);

    r = gana(M[0][0], M[0][1], M[0][2], M[0][3]);
    if(printGana(r, i)) continue;
    r = gana(M[1][0], M[1][1], M[1][2], M[1][3]);
    if(printGana(r, i)) continue;
    r = gana(M[2][0], M[2][1], M[2][2], M[2][3]);
    if(printGana(r, i)) continue;
    r = gana(M[3][0], M[3][1], M[3][2], M[3][3]);
    if(printGana(r, i)) continue;
    r = gana(M[0][0], M[1][0], M[2][0], M[3][0]);
    if(printGana(r, i)) continue;
    r = gana(M[0][1], M[1][1], M[2][1], M[3][1]);
    if(printGana(r, i)) continue;
    r = gana(M[0][2], M[1][2], M[2][2], M[3][2]);
    if(printGana(r, i)) continue;
    r = gana(M[0][3], M[1][3], M[2][3], M[3][3]);
    if(printGana(r, i)) continue;
    r = gana(M[0][0], M[1][1], M[2][2], M[3][3]);
    if(printGana(r, i)) continue;
    r = gana(M[0][3], M[1][2], M[2][1], M[3][0]);
    if(printGana(r, i)) continue;

    fFinalizado = 1;
    for(j=0; j<4; j++){
      for(k=0; k<4; k++){
        if(M[j][k] == '.'){
          fFinalizado = 0;
        }
      }
    }
    if(!fFinalizado){
      printf("Case #%d: Game has not completed\n", i);
      continue;
    }

    printf("Case #%d: Draw\n", i);
  }
  return 0;
}