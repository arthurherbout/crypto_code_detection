#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <math.h>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <algorithm>
#include <numeric>
#include <set>
#include <string.h>
#include <map>
using namespace std;

typedef pair<int, int> pii;
set<pii> good, bad;

bool check(int a, int b) {
    if (a > b) swap(a, b);
    if (a == b) return 0;
    if (b % a == 0) return 1;

    if (b < 2 * a) return !check(a, b - a);
    return 1;
    /*
    if (good.find(pii(a, b)) != good.end()) return 1;
    if (bad.find(pii(a, b)) != bad.end()) return 0;

    for (int i = 1; a * i < b; i++) if (!check(a, b - a * i)) {
        good.insert(pii(a, b)); return 1;
    }
    bad.insert(pii(a, b));
    */
    return 0;
}

int main()
{
    int nc; cin >> nc; for (int cc = 0; cc < nc; cc++) {
        int a1, a2, b1, b2; cin >> a1 >> a2 >> b1 >> b2;
        int res = 0;
        for (int a = a1; a <= a2; a++) for (int b = b1; b <= b2; b++)
            res += check(a, b);
        cout << "Case #" << cc + 1 << ": " << res << endl;
    }
    return 0;
}
