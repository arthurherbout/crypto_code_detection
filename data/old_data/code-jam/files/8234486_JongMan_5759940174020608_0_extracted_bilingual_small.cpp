#include<iostream>
#include<cstring>
#include<algorithm>
#include<sstream>
#include<string>
#include<vector>
#include<cmath>
#include<cstdio>
#include<cstdlib>
#include<fstream>
#include<cassert>
#include<numeric>
#include<set>
#include<map>
#include<queue>
#include<list>
#include<deque>
using namespace std;

int main() {
  int cases;
  cin >> cases;
  for(int cc = 0; cc < cases; ++cc) {
    map<string, int> words;
    int n;
    cin >> n;
    string s;
    getline(cin, s);
    vector<vector<int> > sentences(n);
    for(int i = 0; i < n; ++i) {
      getline(cin, s);
      istringstream inp(s);
      string tok;
      while(inp >> tok) {
        if(words.find(tok) != words.end())
          sentences[i].push_back(words[tok]);
        else {
          int ss = words.size();
          words[tok] = ss;
          sentences[i].push_back(ss);
        }
      }
      sort(sentences[i].begin(), sentences[i].end());
      sentences[i].erase(unique(sentences[i].begin(), sentences[i].end()), sentences[i].end());
    }

    int sol;
    if(n == 2) {
      vector<int> out;
      set_intersection(sentences[0].begin(), sentences[0].end(), sentences[1].begin(), sentences[1].end(), back_inserter(out));
      sol = out.size();
    }
    else {
      int m = n - 2;
      sol = 987654321;
      for(int i = 0; i < (1<<m); ++i) {
        vector<int> english(words.size(), 0), french(words.size(), 0);
        for(int w: sentences[0]) english[w] = 1;
        for(int w: sentences[1]) french[w] = 1;
        for(int j = 0; j < m; ++j) {
          if(i & (1<<j)) {
            for(int w: sentences[j+2]) english[w] = 1;
          }
          else {
            for(int w: sentences[j+2]) french[w] = 1;
          }
        }

        int cand = 0;
        for(int i = 0; i < words.size(); ++i) 
          if(english[i] && french[i])
            cand++;
        sol = min(sol, cand);
      }
    }
    printf("Case #%d: %d\n", cc+1, sol);

  }
}

