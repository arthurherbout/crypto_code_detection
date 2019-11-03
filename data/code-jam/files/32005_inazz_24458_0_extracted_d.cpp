
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

int main(void){
  int cases;
  cin >> cases;
  for(int case_no = 1; case_no <= cases; case_no++){
    int n, m;
    cin >> n;
    vector<pair<int,int> > v1(n-1);
    for(int i=0; i<n-1; i++)
      cin >> v1[i].first >> v1[i].second;
    cin >> m;
    vector<pair<int,int> > v2(m-1);
    for(int i=0; i<m-1; i++)
      cin >> v2[i].first >> v2[i].second;
    vector<int> mapper;
    for(int i=0; i<m; i++)
      mapper.push_back(i+1);
    while(mapper.size() != n)
      mapper.push_back(-1);
    sort(mapper.begin(), mapper.end());
    bool ans = false;
    do{
      int nedge = 0;
      bool ok = true;
      for(int i=0; i<n-1; i++){
	int a = mapper[v1[i].first -1];
	int b = mapper[v1[i].second-1];
	if(a == -1 || b == -1) continue;
	nedge++;
	bool found = false;
	for(int j=0; j<v2.size(); j++){
	  if(v2[j] == make_pair(a,b) || v2[j] == make_pair(b,a)){
	    found = true;
	    break;
	  }
	}
	if(!found) ok = false;
      }
      if(nedge != m-1) ok = false;
      if(ok) ans = true;
    }while(next_permutation(mapper.begin(), mapper.end()));
    cout << "Case #" << case_no << ": ";
    cout << (ans?"YES":"NO");
    cout << endl;
  }
}
