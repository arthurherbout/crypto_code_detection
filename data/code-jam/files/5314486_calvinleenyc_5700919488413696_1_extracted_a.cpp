#include<bits/stdc++.h>

using namespace std;

void solve(){
  int n; cin >> n;
  int p; cin >> p;
  vector<int> freq(p);
  for(int i=0;i<n;i++){
    int t; cin >> t; t = t % p;
    freq[t] += 1;
  }
  vector<int> order;

  for(int e=0;e<freq[0];e++) order.push_back(0);
  if( p == 2){
    for(int e=0;e<freq[1];e++) order.push_back(1);
  }
  if( p == 3){
    if(freq[1] > freq[2]) swap(freq[1], freq[2]);
    for(int e=0;e<freq[1];e++){
      order.push_back(1);
      order.push_back(2);
      freq[2]-=1;
    }
    for(int e=0;e<freq[2];e++){
      order.push_back(2);
    }
  }
  if( p == 4){
    for(int e=0;e<freq[2];e++) order.push_back(2);
    if(freq[1] > freq[3]) swap(freq[1], freq[3]);
    for(int e=0;e<freq[1];e++){
      order.push_back(1);
      order.push_back(3);
      freq[3] -= 1;
    }
    for(int e=0;e<freq[3];e++){
      order.push_back(3);
    }
  }
  assert(order.size() == n);
  int ans = 0;
  int s = 0;
  for(int i=0;i<n;i++){
    if(s == 0){
      ans += 1;
    }
    s = s + order[i];
    s %= p;
  }
  cout << ans << "\n";
}

int main(){
  int T; cin >> T;
  for(int i=0;i<T;i++){
    cout << "Case #" << (i+1) << ": ";
    solve();
  }
}
