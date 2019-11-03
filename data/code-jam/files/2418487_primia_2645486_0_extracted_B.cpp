#include<iostream>
#include<vector>

using namespace std;

int ans;
int E, R, N;
vector<int> v;

void search(int curE, int cur, int best)
{
  if( cur == N ){
    ans = max(ans, best);
    return ;
  }
  for(int ene = 0; ene <= curE; ++ene){
    best += v[cur] * ene;
    search( min(E, curE-ene+R), cur+1, best);
    best -= v[cur] * ene;
  }
  return ;
}

int main()
{
  int T;
  cin >> T;
  for(int tc=1;tc<=T;++tc){
    ans = 0;
    v.clear();
    cin >> E >> R >> N;
    
    for(int i = 0; i < N; ++i){
      int tmp;
      cin >> tmp;
      v.push_back(tmp);
    }
    
    search( E, 0, 0 );
    printf("Case #%d: %d\n", tc, ans);
  }
  return 0;
}

