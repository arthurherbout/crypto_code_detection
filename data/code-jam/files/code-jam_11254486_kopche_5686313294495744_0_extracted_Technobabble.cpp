#include <iostream>
#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>
#include <string>
#include <set>

using namespace std;
#define MAX 1024

int dist(set <string>::iterator it1, set <string>::iterator it2) {
    int answer = 0;
    
    for (set <string>::iterator i = it2; i != it1; ++i) {
        ++answer;
    }
    
    return answer;
}

int main() {
    freopen("C-small-attempt0.in", "rt", stdin);
    freopen("C-small-attempt0.out", "wt", stdout);
    
    int numTests;
    cin >> numTests;
    
    for (int i = 0; i < numTests; ++i) {
        int n;
        
        cin >> n;
        
        vector <string> topicsFirst(n);
        vector <string> topicsSecond(n);
        for (int j = 0; j < n; ++j) {
            cin >> topicsFirst[j] >> topicsSecond[j];
        }
        
        set <string> firstSet(topicsFirst.begin(), topicsFirst.end());
        set <string> secondSet(topicsSecond.begin(), topicsSecond.end());
        int all = (int) firstSet.size() * (int) secondSet.size();
        int cnt = 0;
        int* graph = new int [all];
        memset(graph, 0, sizeof(int) * all);
        for (int j = 0; j < n; ++j) {
            string first = topicsFirst[j];
            string second = topicsSecond[j];
            set <string>::iterator itFirst = firstSet.find(first);
            set <string>::iterator itSecond = secondSet.find(second);
            if (itFirst != firstSet.end() && itSecond != secondSet.end()) {
                graph[dist(itFirst, firstSet.begin()) * ((int) secondSet.size()) + dist(itSecond, secondSet.begin())] = 1;
            }
        }
        for (set <string>::iterator j = firstSet.begin(); j != firstSet.end(); ++j) {
            for (set <string>::iterator k = secondSet.begin(); k != secondSet.end(); ++k) {
                if (*j != *k && !graph[dist(j, firstSet.begin()) * ((int) secondSet.size()) + dist(k, secondSet.begin())]) {
                    ++cnt;
                }
            }
        } 
        
        cout << "Case #" << i + 1 << ": " << cnt << endl;
        delete [] graph;
    }
    
    return 0;
}
