#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <unordered_set>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <ctime>
#define LL long long
using namespace std;

ostream& operator<<(ostream& s, const vector<int>& v) {
    s << '{';
    for (int i = 0 ;i <  v.size(); ++i) {
        s << (i ? "," : "") << v[i];
    }
    return s << '}';
}



string sub2(string s, int b, int e){
  int d = e-b;
  string ss = s.substr(b,d);
  return ss;
}

unordered_set<string> dict;

bool valid(string s){

  // table[i] is true if the first i chars of s is valid
  vector<bool> table(s.length()+1,false);
  table[0] = true;

  for(int i=1; i<=s.length(); i++){

    // Check if S[0..i] is valid

    bool works = false;
    for(int h = 1; h < 12; h++){ // assume everything is less than 20 chars long

      int nsbegin = i-h;
      int nsend = i;

      if(nsbegin < 0) break;

      if(table[i-h]){
        
        // Get S[i-h..i]
        if(nsbegin < 0) nsbegin = 0;
        string ns = sub2(s,nsbegin,nsend);

        bool is = dict.find(ns) != dict.end();
        if(is){
          works = true;
          break;
        }

      }
      
    }

    table[i] = works;


  }

  return table[s.length()];
}



int maxv(vector<int> v){
  if(v.size() == 0) return -10;
  return v[v.size()-1];
}



// All lists where members are 5 or less apart, like {3 8 14}, all members <= n
vector< vector<int> > all_5_apart(int n){

  vector< vector<int> > ret;

  // 0 case
  vector<int> emp;
  ret.push_back(emp);
  vector<int> zer(1,0);
  ret.push_back(zer);

  for(int t = 1; t <= n; t++){

    // ret already contains everything not having t in it
    for(int i=0; i<ret.size(); i++){
      
      if(maxv(ret[i]) > t-5) continue;

      vector<int> nv(ret[i]);
      nv.push_back(t);
      ret.push_back(nv);
      
    }

  }

  return ret;
}



// lev = how many of allp have been changed
bool recur(string S, vector<int> p, int lev){

  if(lev == p.size()) return valid(S);

  // check a-z
  char pl = p[lev];
  bool works = false;
  for(int c='a'; c<='z'; c++){
    if(pl == c) continue;
    S[lev] = c;
    if(recur(S,p,lev+1)){
      works = true;
      break;
    }
    S[lev] = pl;
  }

  return works;

}



int bf(string S){
  int lS = S.length();
  vector< vector<int> > allp = all_5_apart(lS);

  int min = 100000;
  for(int i=0; i<allp.size(); i++){
    if(allp[i].size() < min){
      if(recur(S,allp[i],0)){
        min = allp[i].size();
      }
    }
  }

  return min;
}




main(){
  
  int nwords;
  cin>>nwords;
  while(nwords--){
    string w;
    cin>>w;
    dict.insert(w);
  }

  int nc;
  cin>>nc;
  for(int i=1; i<=nc; i++){
    string S;
    cin>>S;
    int ans = bf(S);
    cout << "Case #" << i << ": " << ans << endl;
  }

}

