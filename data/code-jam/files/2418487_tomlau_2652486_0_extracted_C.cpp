#include <stdio.h>
#include <iostream>

#define zero(x) memset(x, 0, sizeof(x))

using namespace std;

int main()
{
  int T, R, N, M, K;
  cin >> T;
  cin >> R >> N >> M >> K;
  int pr[16];
  bool can_get[128];
  int g[16];

  cout << "Case #1:" << endl;

  for (int r=0; r<R; r++) {
    
    for (int i=0; i<K; i++)
      cin >> pr[i];

    for (int i=0; i<N; i++)
      g[i] = 2;

    while (g[0] <= M) {
      zero(can_get);
      for (int i=0; i<1<<N; i++) {
        int prod=1;
        for (int j=0; j<N; j++)
          if (i & (1<<j)) prod *= g[j];
        can_get[prod] = true;
      }

      bool all=true;
      for (int i=0; i<K; i++)
        if (!can_get[pr[i]]) { 
          all=false;
          break;
        }

      if (all) {
        for (int i=0; i<N; i++)
          cout << g[i];
        cout << endl;
        break;
      }

      int ind=N-1;
      g[ind]++;
      while (g[ind] > M && ind > 0) {
        g[ind] = 2;
        ind--;
        g[ind]++;
      }
    }
  }
  
  return 0;
}
