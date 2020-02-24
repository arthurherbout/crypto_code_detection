#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <fstream>

using namespace std;

typedef long long Long;

int maxRings(Long r, Long t)
{
    int numDrawn = 0;
    Long ringArea = (r + 1) * (r + 1) - r * r;
    while (t >= ringArea) {
        t -= ringArea;
        r += 2;
        ringArea = (r + 1) * (r + 1) - r * r;
        numDrawn++;
    }
    return numDrawn;
}

int main()
{
    istream& input = cin;
    // ifstream input("sample");
    int T;
    input >> T;
    for (int t = 0; t != T; ++t) {
        Long r, tt;
        input >> r >> tt;
        cout << "Case #" << (t + 1) << ": " << maxRings(r, tt) << endl;
    }
    return 0;
}
