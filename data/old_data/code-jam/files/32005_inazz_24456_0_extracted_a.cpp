
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <queue>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <complex>
#include <cctype>
#include <cstdlib>
#include <cstring>

using namespace std;

#define INF 12345678

int main(void){
  int cases;
  cin >> cases;
  for(int case_no = 1; case_no <= cases; case_no++){
    int n;
    int data[2][4] = {{0, INF, 0, INF}, {0,INF,0,INF}};
    cin >> n;
    vector<pair<int,int> > v;
    while(n--){
      int a,b;
      string s;
      cin >> a >> b >> s;
      if(s == "NOT"){
	cin >> s;
	v.push_back(make_pair(a,b));
      }else{
	data[0][1] = min(data[0][1], a);
	data[0][2] = max(data[0][2], a);
	data[1][1] = min(data[1][1], b);
	data[1][2] = max(data[1][2], b);
      }
    }
    vector<bool> used(v.size(), false);
    if(data[0][1] != INF){
      for(int i=0; i<v.size(); i++){
	if(data[0][1] <= v[i].first && v[i].first <= data[0][2]){
	  if(v[i].second < data[1][1]){
	    data[1][0] = max(data[1][0], v[i].second);
	  }else{
	    data[1][3] = min(data[1][3], v[i].second);
	  }
	  used[i] = true;
	}else if(data[1][1] <= v[i].second && v[i].second <= data[1][2]){
	  if(v[i].first < data[0][1]){
	    data[0][0] = max(data[0][0], v[i].first);
	  }else{
	    data[0][3] = min(data[0][3], v[i].first);
	  }
	  used[i] = true;
	}
      }
    }
    cout << "Case #" << case_no << ": " << endl;
    cin >> n;
    while(n--){
      int a,b;
      cin >> a >> b;
      if(data[0][1] == INF){
	bool ok = false;
	for(int i=0; i<v.size(); i++){
	  if(v[i] == make_pair(a,b))
	    ok = true;
	}
	if(ok){
	  cout << "NOT BIRD" << endl;
	}else{
	  cout << "UNKNOWN" << endl;
	}
      }else{
	if(data[0][1] <= a && a <= data[0][2] &&
	   data[1][1] <= b && b <= data[1][2]){
	  cout << "BIRD" << endl;
	}else if(a <= data[0][0] || data[0][3] <= a ||
		 b <= data[1][0] || data[1][3] <= b){
	  cout << "NOT BIRD" << endl;
	}else{
	  bool ok_out = false;
	  for(int i=0; i<v.size(); i++){
	    if(used[i]) continue;
	    if((a - v[i].first ) * (v[i].first  - data[0][2])>= 0 &&
	       (b - v[i].second) * (v[i].second - data[1][2])>= 0)
	      ok_out = true;
	  }
	  if(ok_out){
	    cout << "NOT BIRD" << endl;
	  }else{
	    cout << "UNKNOWN" << endl;
	  }
	}
      }
    }
  }
}
