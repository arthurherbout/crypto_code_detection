#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <functional>
#include <string>

#define all(x) (x).begin(), (x).end()
#define zero(x) memset(x, 0, sizeof(x))

using namespace std;

#define MAX 256  // 256

short hk[MAX];  // have keys
short total[MAX]; // total keys needed - for quick check
short cht[MAX];  // key to open chest
short chkc[MAX]; // count of keys for each chest
short chk[MAX][400];  // keys, contained in chest
bool gives_key[MAX][MAX];
bool no_better[MAX][MAX];
short seq[MAX];
bool  open[MAX];

int N, t;

bool go(int index, int chest) {
  seq[index] = chest;
  if (index == N-1) return true;

  int i;
  open[chest] = true;
  hk[cht[chest]]--;

  for (i=0;i<chkc[chest];i++)
    hk[chk[chest][i]]++;

  for (i=1;i<=N;i++)
     if (!open[i] && hk[cht[i]])
        if (i > chest || gives_key[chest][cht[i]]) {
          // check if no similar chest was tried
           bool proceed = true;
           for (int j=1; j<i; j++)
             if (!open[j] && hk[cht[j]]) {
               if (no_better[i][j]) {
                 proceed = false;
                 break;
               }
             }
           if (proceed)
             if (go(index+1, i)) return true;
        }

  for (i=0;i<chkc[chest];i++)
      hk[chk[chest][i]]--;
     
  open[chest] = false;
  hk[cht[chest]]++;

  return false;
}

int main()
{                /*/      TEST DATA generation
  N = 200;
  cout << 25 << endl;
  for (int i=0; i<25; i++) {
    cout << "10 " << N << endl;
    for (int j=0; j<10; j++)
      cout << (rand() % 1 + 1) << " ";
    cout << endl;
    for (int j=0; j<N; j++) {
      int count = (rand() % 3);
      cout << (rand() % 1 + 1) << " " << count;
      for (int k=0; k<count; k++)
        cout << " " << (rand() % 1 + 1);
      cout << endl;
    }
  }

  return 0;
                   /**/


  int T, key;
  cin >> T;

  for (t=1; t<=T; t++)	{
    int K, i;
	  cin >> K >> N;
    zero(hk);
    zero(chk);
    zero(total);
    for (i=0;i<K;i++) {
      cin >> key;
      hk[key]++;
      total[key]++;
    }

    zero(gives_key);

    for (i=1;i<=N;i++) {
      cin >> cht[i] >> chkc[i];
      total[cht[i]]--;

      for (int j=0; j<chkc[i]; j++) {
        cin >> chk[i][j];
        total[chk[i][j]]++;
        gives_key[i][chk[i][j]] = true;
      }
    }

    zero(no_better);
    for (i=1; i<=N; i++)
      for (int j=i+1; j<=N; j++) {
         bool same = (cht[j] == cht[i]);
         if (same)
           for (int k=0; k<chkc[j]; k++)
             if (!gives_key[i][chk[j][k]]) {
               same = false;
               break;
             }
        if (same)
          no_better[j][i] = true;
      }


    bool found = false;
    cout << "Case #" << t << ":";
    zero(open);
    
   bool bgo=true;
   for (int i=1; i<=N; i++)
     if (total[i] < 0) { bgo=false; break; }
   if (bgo)
    for (i=1;i<=N;i++)
      if (hk[cht[i]] && !open[i]) {
        if (go(0, i)) {
          for (int k=0; k<N; k++)
            cout << " " << seq[k];
          found = true;
          break;
        }
      }

     if (!found) cout << " IMPOSSIBLE";
     cout << endl;
	}
  
	return 0;
}
