#include<bits/stdc++.h>

using namespace std;

typedef long long ll;

typedef pair<int, char> pii;

char ans[1005];

void solve(){
  int n; cin >> n;
  int r,o,y,g,b,v; cin >> r >> o >> y >> g >> b >> v;

  if(2 * o > b || 2 * g > r || 2 * v > y){
    cout << "IMPOSSIBLE";
    return;
  }
  b -= o;
  r -= g;
  y -= v;
  int nprime = r + b + y;
  if(r * 2 > nprime || b * 2 > nprime || y * 2 > nprime){
    cout << "IMPOSSIBLE";
    return;
  }
  vector<pii> colors;
  colors.push_back(pii(b, 'B'));
  colors.push_back(pii(r, 'R'));
  colors.push_back(pii(y, 'Y'));
  sort(colors.begin(), colors.end());
  reverse(colors.begin(), colors.end());
  int c = 0;
  int pos = 0;
  while(c < 3){
    ans[pos] = colors[c].second;
    colors[c].first -= 1;
    while(colors[c].first == 0) c++;
    pos = pos + 2;
    if(pos >= nprime) pos = 1;
  }
  map<char, char> ee;
  ee['B'] = 'O';
  ee['R'] = 'G';
  ee['Y'] = 'V';
  map<char, int> freq;
  freq['O'] = o;
  freq['G'] = g;
  freq['V'] = v;
  for(int i=0;i<nprime;i++){
    cout << ans[i];
    if(freq[ee[ans[i]]] != 0){
      freq[ee[ans[i]]] -= 1;
      cout << ee[ans[i]];
      cout << ans[i];
    }
  }
}

int main(){
  int T; cin >> T;
  for(int i=0;i<T;i++){
    cout << "Case #" << (i+1) << ": "; solve(); cout << "\n";
  }
}
