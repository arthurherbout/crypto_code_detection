#include <iostream>
#include <string>
#include <sstream>
#include <math.h>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <algorithm>
#include <numeric>
#include <set>
#include <string.h>
using namespace std;

int main()
{
    int nc; cin >> nc; for (int cc = 0; cc < nc; cc++) {
        long long k; int n, r; cin >> r >> k >> n; vector<int> v(n); for (int i = 0; i < n; i++) cin >> v[i];
        vector<long long> psum; vector<int> poses; int pos = 0; int rep = -1;
        while (1) {
            for (int i = 0; i < poses.size(); i++) if (poses[i] == pos) { rep = i; break; } if (rep != -1) break;
            poses.push_back(pos);

            int start = pos;
            long long tot = 0;
            while (1) { 
                if (tot + v[pos] > k) break; 
                tot += v[pos]; 
                pos = (pos + 1) % n; 
                if (pos == start) break; 
            }
            psum.push_back(tot);
        }

        long long res = 0;

//        for (int i = 0; i < psum.size(); i++) cout << poses[i] << ' ' << psum[i] << endl;
        for (int i = 0; i < min(rep, r); i++) res += psum[i];
//        cout << res << endl;

        int size = psum.size() - rep;
        long long allsum = accumulate(psum.begin() + rep, psum.end(), 0);
        r -= min(rep, r);
//        cout << r << endl;
        res += (r / size) * allsum + accumulate(psum.begin() + rep, psum.begin() + rep + (r % size), 0);
        cout << "Case #" << cc + 1 << ": " << res << endl;
    }
    return 0;
}
