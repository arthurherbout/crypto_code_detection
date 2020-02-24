#include <bits/stdc++.h>
using namespace std;

int N,L;
string G[120],B;

int main() {
  int t; cin>>t; for(int zz=1;zz<=t;zz++) {
    cin>>N>>L;
    for(int i=0;i<N;i++) cin>>G[i];
    cin>>B;
    for(char c:B) assert(c=='1');

    for(int i=0;i<N;i++) {
      for(char c:G[i]) if(c=='0') goto good;
      goto die;
    good:;
    }

    if(L==1) {
      printf("Case #%d: 1? 1\n",zz); continue;
    }
    printf("Case #%d: 1010101010101010101010101010101010101010101010101010101010101010?101010101010101010101010101010101010101010101010101010101010101 ",zz);
    L--;
    for(;L--;) cout<<'?';
    cout<<endl;
    continue;
  die:
    printf("Case #%d: IMPOSSIBLE\n",zz);
  }
}
