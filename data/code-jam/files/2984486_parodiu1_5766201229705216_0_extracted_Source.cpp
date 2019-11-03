#include<iostream>
#include<vector>
#include<algorithm>
#include<stdlib.h>
#include<fstream>
#include<string>
#include<iomanip>
#include<utility>
#include<unordered_set>
#include<unordered_map>

using namespace std;

class Case{
    int n; string s;
public:
    Case(int nn, string ss = string()) :
        n(nn), s(ss)
    {
    }

    friend ostream& operator<<(ostream& out, Case& c)
    {
        return out << "Case #" << c.n << ": " << c.s ;
    }
};

void traverse(unordered_map<int, unordered_set<int>> & data, int i, unordered_set<int> & res) {
    res.insert(i);
    for (int node : data[i]) {
        if (res.find(node) != res.end())
        {
            traverse(data, node, res);
        }
    }
}


int getNodeCount(vector<unordered_set<int>>& data, int root, int parent = -1) {
    int count = 1;
    for (int i : data[root]) {
        if (i != parent)
        {
            count += getNodeCount(data, i, root);
        }
    }
    return count;
}

int binarize(vector<unordered_set<int>>& data, int root, int parent = -1) {
    vector<int> set;
    for (int i : data[root])
    {
        if (i != parent)
        set.push_back(i);
    }
    if (set.size() == 0)
        return 0;
    if (set.size() == 1)
        return getNodeCount(data, set[0], root);
    int m = 0x7FFFFFFF;
    for (int i = 0; i < set.size() - 1; i++) {
        for (int j = i + 1; j < set.size(); j++) {
            int cnt = 0;
            for (int k : set) {
                if (k == set[i] || k == set[j]) {
                    cnt += binarize(data, k, root);
                }
                else {
                    cnt += getNodeCount(data, k, root);
                }
            }
            m = min(m, cnt);
        }
    }
    return m;
}



int main()
{
    ifstream cin("B-small-attempt0.in");
    ofstream cout("a.out");
    int t, n;
    cin >> t;
    for (int test = 1; test <= t; test++)
    {
        cin >> n;
        vector<unordered_set<int>> data(n);
        for (int i = 0; i < n; i++) {
            data[i] = unordered_set<int>();
        }
        for (int i = 0; i < n-1; i++) {
            int x, y;
            cin >> x >> y;
            x--; y--;
            data[x].insert(y);
            data[y].insert(x);
        }
        int m = 0x7FFFFFFF;
        for (int i = 0; i < n; i++) {
            m = min(binarize(data, i), m);
        }
        cout << Case(test, "") << m << "\n";
    }

   
    

  //  system("PAUSE");

}