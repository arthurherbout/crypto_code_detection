#include<iostream>
#include<cstdio>
#include<cstring>
#include<string>
#include<vector>
#include<algorithm>
#include<map>
using namespace std;

#define FOR(i, a, b) for (int i(a), _b(b); i <= _b; i++)
#define FORD(i, a, b) for (int i(a), _b(b); i >= _b; i--)
#define REP(i, n) for (int i(0), _n(n); i < _n; i++)
#define REPD(i, n) for (int i((n)-1); i >= 0; i--)
#define CLR(x, with) memset((x), with, sizeof(x))
#define ALL(x) (x).begin(), (x).end()

typedef long long LL;
typedef unsigned long long ULL;

int N, M;

int main() {
    int T;
    cin >> T;

    FOR(t, 1, T)
    {
        printf("Case #%d: ",t);

        cin >> N >> M;
        //cout<<N<<" "<<M<<endl;

        string s;
        map<string, int> was;
        REP(i, N)
        {
            cin >> s; //cout<<s<<endl;
            size_t pos = 0, next, sz = s.size();
            do
            {
                next = s.find_first_of('/', ++pos);
                if (next == string::npos)
                {
                    next = s.size();
                }
                pos = next;
                string sub = s.substr(0, pos);
                //cout << "Sub = " <<sub << endl;
                was[sub]++;
            } while (next < sz);
        }

        int res = 0;
        REP(i, M)
        {
            cin >> s; //cout << s << endl;
            size_t pos = 0, next, sz = s.size();
            do
            {
                next = s.find_first_of('/', ++pos);
                if (next == string::npos)
                {
                    next = s.size();
                }
                pos = next;
                string sub = s.substr(0, pos);
                if (!was.count(sub))
                {
                    res++;
                    was[sub]++;
                }
            } while (next < sz);
        }
        //cout<<"cnt = "<<res<<endl;
        printf("%d\n", res);
    }

    return 0;
}
