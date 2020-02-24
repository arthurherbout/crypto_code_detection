#include <bits/stdc++.h>

#define rep(i, n) for(int i = 0;i < n;i++)
#define REP(i, a, n) for(int i = a;i < n;i++)
#define all(i) i.begin(), i.end()

using namespace std;

#define int long long

int toint(string s){
    stringstream ss(s);
    int t;
    ss >> t;
    return t;
}

double PI = acos(-1);

int P = 100000000000;

signed main(){

    int T;
    cin >> T;

    rep(_, T){
        cout << "Case #" << _ + 1 << ": ";

        int n, k;
        cin >> n >> k;
        double sx;
        cin >> sx;

        int x = sx * P;

        vector<int> a(n);
        rep(i, n){
            double sa;
            cin >> sa;
            a[i] = sa * P;
        }
        a.push_back(1e17);

        sort(all(a));
        vector<long double> ba(n);

        [&](){
        while(1){
            int mi = a[0];

            REP(i, 1, a.size()){
                if(mi != a[i]){
                    if((a[i] - mi) * i <= x){
                        rep(j, i){
                            x -= a[i] - a[j];
                            a[j] = a[i];
                        }
                    }else{

                        rep(j, n){
                            ba[j] = a[j];
                        }
                        rep(j, i){
                            ba[j] = ba[j] + double(x) / i;
                        }
                        rep(j, n){
                            ba[j] /= P;
                            ba[j] = min(1.0l , ba[j]);
                        }
                        return;

                    }                    
                    break;
                }
            }

            sort(all(a));
        }
        }();

        long double ans = 1.0;
        rep(i, n){
            ans *= ba[i];
        }
        cout << fixed << setprecision(6) << ans << endl;


    }



    return 0;
}