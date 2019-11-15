#include <bits/stdc++.h>
using namespace std;
#define mp(x,y) make_pair((x),(y))
#define pb(x) push_back(x)
#define sz(x) ((int)(x).size())
//#define debug(x) (cerr << #x << " = " << (x) << endl)
#define debug(x)
template<class I> void debug2(I b, I e) { while (b != e) cerr << *b++ << " "; cerr << endl; }

double solve() {
    int k, l, s;
    cin >> k >> l >> s;
    string keys, target;
    cin >> keys >> target;
    
    double res = 0;
    
    int freq[26];
    memset(freq,0,sizeof(freq));
    for (int i = 0; i < k; i++) {
        freq[keys[i]-'A']++;
    }
    
    // Check if all leters present
    for (int i = 0; i < l; i++) {
        if (freq[target[i] - 'A'] == 0) {
            return 0;
        }
    }
    
    int mdist = l;
    for (int d = 1; d < l; d++) {
        if (target.substr(d) == target.substr(0, l - d)) {
            mdist = d;
            break;
        }
    }
    
    debug(mdist);
    
    int mxban = (s - (l - 1) + mdist - 1) / mdist;
    
    double p = 1.0;
    for (int i = 0; i < l; i++) {
        p *= (double)freq[target[i]-'A'] / k;
    }
    
    debug(p);
    debug(mxban);
//    expected[0] = 0;
//    for (int i = 1; i <= s; i++) {
//        
//    }
    
//    double sufprob[100 + 1];
//    sufprob[0] = 1.00;
//    for (int i = 1; i <= l; i++) {
//        sufprob[i] = ((double)freq[target[l-i]-'A'] / k) * sufprob[i - 1];
//    }
    
//    debug2(sufprob, sufprob + l + 1);
    
//    double dp[100 + 1];
//    dp[0] = 0.0;
//    for (int i = 1; i <= s; i++) {
//        dp[i] = 0.0;
//        if (i >= l) {
//            dp[i] += dp[i - l] + 1 * p;
//        }
//        for (int j = i - 1; j > 0 && j > i - l; j--) {
//            int d = j - i;
//            if (target.substr(0, l - d) == target.substr(d)) {
//                dp[i] += dp[i - d] * (1.0 - sufprob[d]);
//            }
//        }
//    }
    
    double expected = 0;
    int counter = 0;
    
//    for (int m = 0; m < (1 << (s - l + 1)); m++) {
////        debug(m);
//        bool ok = true;
//        for (int i = 0; i < s; i++) {
//            for (int j = i + 1; ok && j < s; j++) {
//                if ((m & (1 << i)) && (m & (1 << j))) {
////                    debug(i);
////                    debug(j);
////                    debug(m);
//                    if (j - i < mdist) {
//                        ok = false;
//                        break;
//                    }
//                }
//            }
//        }
//        if (ok) {
////            debug(m);
//            int t = __builtin_popcount(m);
//            double pr = 1.0;
//            int head = -1;
//            for (int i = 0; i < s; i++) {
//                if (m & (1 << i)) {
//                    head = i;
//                }
//                if (head != -1) {
//                    if (i - head >= l) {
//                        head = -1;
//                    } else {
//                        pr *= (double)freq[target[i - head]-'A'] / k;
//                    }
//                }
//            }
//            cout << ((t&1) ? t * pr : -t * pr) << endl;
//            expected += (t&1) ? t * pr : -t * pr;
//            counter++;
//        }
//    }
    
    int maxp = 1;
    for (int i = 0; i < s; i++) maxp *= k;
    
    char t[100];
    memset(t,0,sizeof(t));
    int sum = 0;
    int mx = 0;
    for (int m = 0; m < maxp; m++) {
        int tmp = m;
        for (int i = 0; i < s; i++) {
            t[i] = keys[tmp % k];
            tmp /= k;
        }
        //cout << t << endl;
        int tmpmx = 0;
        string tmps = string(t);
        for (int i = 0; i + l - 1 < s; i++) {
            if (tmps.substr(i, l) == target) {
                sum++;
                tmpmx++;
            }
        }
        mx = max(mx, tmpmx);
        counter++;
    }
    debug(sum);
    debug(counter);
    debug(mx);
    //expected /= counter;
    expected = (double)sum / counter;
    
    debug(expected);
    
    return mx - expected;
}

int main() {
//    freopen("B.in", "r", stdin);
    
    int tests;
    cin >> tests;
    for (int test = 1; test <= tests; test++) {
        printf("Case #%d: ", test);
        cout << solve() << endl;
    }
    
    return 0;
}
