#include <iostream>
#include <vector>
#include <string>
#include <set>

using namespace std;

int f1(int a) {
    int b = a - 1;
    return (b + 1) / 2;
}

int f2(int a) {
    int b = a - 1;
    return b / 2;
}

int main() {
    //ifstream cin("input.in");
    //ofstream cout("output.txt");
    int t;
    long long n, m;
    cin >> t;
    for (int t1 = 0; t1 < t; t1++) {
        cout << "Case #" << t1 + 1 << ": ";
        cin >> n >> m;
        multiset<int> v;
        v.insert(-n);
        for (int i = 0; i < m - 1; i++) {
            int a = -(*(v.begin()));
            v.erase(v.begin());
            v.insert(-(f1(a)));
            v.insert(-(f2(a)));
        }
        int a = -*(v.begin());
        cout << f1(a) << " " << f2(a) << endl;
    }
    return 0;
}
