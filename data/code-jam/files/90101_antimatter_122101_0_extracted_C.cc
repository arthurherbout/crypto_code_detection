#include <cstdio>
#include <cstdlib>
#include <cstring>
using namespace std;

int n[501][20];
char buf[1000];
int N;
char* str="welcome to code jam";

int main() {
  fgets(buf, 1000, stdin);
  sscanf(buf, "%d", &N);
  int cn = 1;
  while (N--) {
    memset(n,0,sizeof(n));
    fgets(buf, 1000, stdin);

    for (int i = 0; buf[i]; i++) {
      if (buf[i] == str[0]) {
	n[i][1] = 1;
      }
      if (i > 0) {
	for (int j = 1; j < 20; j++) {
	  n[i][j] += n[i-1][j];
	  n[i][j] %= 10000;
	  
	  if (buf[i] == str[j]) {
	    n[i][j+1] += n[i-1][j];
	    n[i][j+1] %= 10000;
	  }
	}
      }
    }

    int answer = n[strlen(buf) - 1][19];
    printf("Case #%d: %04d\n", cn++, answer);
  }
}
