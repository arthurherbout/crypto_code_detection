#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <cassert>
#include <vector>
#include <set>
using namespace std;

int N;
int adj[1009][1009];
int numAdj[1009];
int sizeCache[1009];

int calcSize(int node, int par) {
  int total = 1;
  for (int a=0; a<numAdj[node]; a++) {
    int other = adj[node][a];
    if (other != par) {
      total += calcSize(other, node);
    }
  }
  sizeCache[node] = total;
  return total;
}

int size(int node, int par) {
  return sizeCache[node];
}

int needDel(int node, int par) {
  int numC = (par == 0) ? (numAdj[node]) : (numAdj[node]-1);
  assert(numC >= 0);
  if (numC == 0) {
    return 0;
  } else if (numC == 1) {
    if (adj[node][0] == par) {
      assert(adj[node][1] != par);
      return size(adj[node][1], node);
    } else {
      return size(adj[node][0], node);
    }
  } else {
    int childSize = size(node,par)-1;
    //vector<int> sizes;
    //vector<int> needs;
    vector<int> gains;
    for (int a=0; a<numAdj[node]; a++) {
      int other = adj[node][a];
      if (other != par) {
        //sizes.push_back(size(other, node));
        //needs.push_back(needDel(other, node));
        //gains.push_back(make_pair(sizes.back()-needs.back(), 
        int oSize = size(other, node);
        int oNeed = needDel(other, node);
        gains.push_back(oNeed - oSize);
      }
    }
    partial_sort(gains.begin(), gains.begin()+2, gains.end());
    return childSize + gains[0] + gains[1];
  }
}

int solve() {
  cin >> N;
  memset(numAdj, 0, sizeof numAdj);
  for (int edge=0; edge<N-1; edge++) {
    int Xi, Yi;
    cin >> Xi >> Yi;
    adj[Xi][numAdj[Xi]] = Yi;
    numAdj[Xi]++;
    adj[Yi][numAdj[Yi]] = Xi;
    numAdj[Yi]++;
  }
  int minAns = N+1;
  for (int root=1; root<=N; root++) {
    calcSize(root, 0);
    minAns = min(minAns, needDel(root, 0));
  }
  assert(minAns <= (N-1));
  return minAns;
} 

int main() {
  int T;
  cin >> T;
  for (int testcase=1; testcase<=T; testcase++) {
    int ans = solve();
    cout << "Case #" << testcase << ": " << ans << endl;
  }
  return 0;
}
