# include <stdio.h>
# include <iostream>

using namespace std;

int b[50][50];
int baux[50][50];
int array[2000];
int r, c;
int pd[1 << 17][4][4];

int encode(){
  int s = 0;
  for(int i=r-1; i>=0; i--)
    for(int j=c-1; j>=0; j--)
      s = (s*2 + baux[i][j]);
  return s;
}

void decode(int bm){  
  for (int i=0; i<r*c; i++){
    array[i] = bm % 2;
    bm /= 2;
  }
  for (int i=0; i<r; i++)
    for (int j=0; j<c; j++)
      baux[i][j] = array[i*c + j];
}
bool ok(int i, int j){
  return i >= 0 && i < r && j>=0 && j < c && baux[i][j] == 0;
}

int bt(int bm, int i, int j){

//   printf("%d %d %d\n",bm,i,j);

  if( pd[bm][i][j] != -1 ) return pd[bm][i][j];

  decode(bm);

  int res = 0; //0=derrota

  //Procura uma posicao de vitoria
  for (int di=-1; di<=1; di++)
    for (int dj=-1; dj<=1; dj++){

      if (!ok(i+di, j+dj)) continue;
      baux[i+di][j+dj] = 1;

      if (bt(encode(), i+di, j+dj) == 0){
	res = 1;
	goto break2;
      }

      baux[i+di][j+dj] = 0;
    }
 break2:
  pd[bm][i][j] = res;
  return res;
}

int main (){

  int t, cases = 1;

  char str[50];
  int srci, srcj;

  scanf("%d", &t);

  while(t--){
    scanf("%d %d", &r, &c);

    for (int i=0; i<r; i++)
      for (int j=0; j<c; j++)
	b[i][j] = 0;

    for (int i=0; i<r; i++){
      scanf("%s", str);
      for (int j=0; j<c; j++){
	if (str[j] == '#')
	  b[i][j] = 1;
	if (str[j] == 'K'){
	  b[i][j] = 1;
	  srci = i;
	  srcj = j;
	}
      }
    }

    for (int i=0; i<r; i++)
      for (int j=0; j<c; j++)
	baux[i][j] = b[i][j];

//     for (int i=0; i<r; i++){
//       for (int j=0; j<c; j++)
// 	printf("%d", baux[i][j]);
//       printf("\n");
//     }

    int S = (1 << (r*c));
    for (int s=0; s<=S; s++)
      for (int i=0; i<r; i++)
	for (int j=0; j<c; j++)
	  pd[s][i][j] = -1;

    int ans = bt(encode(), srci, srcj); 
    printf("Case #%d: ", cases++);
    if (ans == 1)
      printf("A\n");
    else 
      printf("B\n");
    
  }

  return 0;
}
