#include <iostream>
#include <string>
#include <cstring>
#include <algorithm>
#include <vector>
#include <cstdio>
#include <cmath>
#include <set>
#include <map>
#include <queue>
using namespace std;

typedef long long int64;
typedef pair<int, int> P;

#define M 3000
#define N 3000

int n, m;

int graph[200][200];

set<string> st;

char change[][3] = {
    "o0", "i1", "e3", "a4", "s5", "t7", "b8", "g9" 
};

int T;
string s;
int k;

void bt(string s, int idx) {
    if(idx == 2) {
        st.insert(s);
    } else {
        bt(s, idx + 1);
        for(int i = 0; i < 8; i ++) {
            if(s[idx] == change[i][0]) {
                s[idx] = change[i][1];
            }
        }
        bt(s, idx + 1);
    }
}

int indeg[200], outdeg[200];
bool visited[200];

bool dfs(int s) {
    if(visited[s]) return false;
    visited[s] = true;
    bool result = (outdeg[s] > indeg[s]);
    for(int i = 0; i < 200; i ++) {
        if(graph[s][i] || graph[i][s])
            if(dfs(i)) {
                result = true;
            }
    }
    return result;
}

int main() {
    cin >> T;
    for(int I = 1; I <= T; I ++) {
        cin >> k >> s;
        st.clear();
        n = s.length();
        for(int i = 0; i + 1 < n; i ++) {
            bt(s.substr(i, 2), 0);
        }
        vector<string> v(st.begin(), st.end());
        n = m = v.size();
        memset(graph, 0, sizeof graph);
        int result = 0;
        memset(indeg, 0, sizeof indeg);
        memset(outdeg, 0, sizeof outdeg);
        for(int i = 0; i < n; i ++) {
            result ++;
            graph[v[i][0]][v[i][1]] ++;
            indeg[v[i][1]] ++;
            outdeg[v[i][0]] ++;
        }
        for(int i = 0; i < 200; i ++) {
            if(outdeg[i] > indeg[i]) {
                result += outdeg[i] - indeg[i];
            }
        }
        memset(visited, 0, sizeof visited);
        for(int i = 0; i < 200; i ++) {
            if(!visited[i] && indeg[i] + outdeg[i]) {
                if(!dfs(i)) {
                    result ++;
                }
            }
        }
        cout << "Case #" << I << ": " << result << endl;
    }
    return 0;
}
















