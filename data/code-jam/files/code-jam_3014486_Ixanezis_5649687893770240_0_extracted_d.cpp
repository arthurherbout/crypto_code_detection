#include <iostream>
#include <cstring>
#include <string>
#include <sstream>
#include <algorithm>
#include <set>
#include <vector>

using namespace std;

int graph[178][27];

int build(const vector<string>& s) {
    /*
    cerr << "checking: " << endl;
    for (const string& str : s)
        cerr << str << endl;
        */

    memset(graph, 0, sizeof graph);
    int last = 2;

    for (const string& str : s) {
        int cur = 1;
        for (const char c : str) {
            if (graph[cur][c-'A']) {
            } else {
                graph[cur][c-'A'] = last;
                ++last;
            }
            cur = graph[cur][c-'A'];
        }
    }

    return last - 1;
}

set<vector<set<string>>> used;

vector<string> split[5];
int mx, num;

void DFS(const vector<string>& s, int pos, int servers) {
    if (pos == int(s.size())) {
        int sum = 0;
        //cerr << "split: " << endl;
        for (int i=0; i<servers; ++i) {
            if (split[i].empty())
                sum = -111111111;
            sum += build(split[i]);

            /*
            cerr << "server " << i << ": " << endl;
            for (size_t u=0; u<split[i].size(); ++u) {
                cerr << split[i][u] << ' ';
            }
            cerr << endl;
            */
        }

        if (sum > mx) {
            mx = sum;
            num = 1;
        } else if (sum == mx) {
            ++num;
        }
        return;
    }

    for (int i=0; i<servers; ++i) {
        split[i].push_back(s[pos]);
        DFS(s, pos+1, servers);
        split[i].pop_back();
    }
}

int main() {
	int t; cin >> t;

	for (int e=1; e<=t; ++e) {
		cout << "Case #" << e << ": ";
        int m,n; cin >> m >> n;
        vector<string> s;
        for (int i=0; i<m; ++i) {
            string st; cin >> st;
            s.push_back(st);
        }

        sort(s.begin(), s.end());

        for (int i=0; i<n; ++i)
            split[i].clear();

        mx = 0;
        DFS(s, 0, n);

        cout << mx << ' ' << num << endl;
	}

	return 0;
}


