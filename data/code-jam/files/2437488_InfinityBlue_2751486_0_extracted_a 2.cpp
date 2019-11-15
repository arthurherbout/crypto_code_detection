#include <iostream>
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <set>
#include <vector>
#include <queue>
#include <stack>
#include <cmath>
#include <map>
using namespace std;
#define INF 0x73737373
#define EPS 1e-8
#define lson l, m, rt<<1
#define rson m+1, r, rt<<1|1
map <pair<int, int>, bool> hash;
int main()
{
    int t;
    freopen("A-small-attempt0.in", "r", stdin);
    freopen("a.out", "w", stdout);
    scanf("%d", &t);
    for(int ca = 1; ca <= t; ca++){
        hash.clear();
        string s;
        int n, tmp = 0, ret = 0;
        cin >> s >> n;
        for(int i = 0; i < s.size(); i++)
        {
            for(int j = i; j < s.size(); j++)
            {
                int tmp = 0;
                for(int k = i; k <= j; k++){
                    if(s[k] != 'a' && s[k] != 'i' && s[k] != 'e' && s[k] != 'o' && s[k] != 'u') tmp++;
                    else tmp = 0;
                    if(tmp >= n) {
                        if(!hash[make_pair(i, j)]){
                            ret++;
                            hash[make_pair(i, j)] = true;
                        }
                    }
                }
            }
        }
        printf("Case #%d: %d\n", ca, ret);

    }
	return 0;
}
