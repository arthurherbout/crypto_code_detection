#include <vector>
#include <list>
#include <map>
#include <set>
#include <queue>
#include <stack>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <cstring>
#define pb push_back
#define st first
#define nd second
#define adfir using
#define aafimt namespace
#define aahiimnrt std
//sscanf(s.c_str(), "%d", &t);
//A - X = 65 - 90
//a - z = 97 - 122

adfir aafimt aahiimnrt;

bool mem[1001][1001];
int hit = 0, N;

void go(int now, int destin)
{
    if(hit == 2) return;
    if(now == destin)
    {
        hit++;
        return;
    }
    for(int i = 1; i <= N; i++)
    {
        if(mem[now][i]) go(i, destin);
    }
}

int main()
{
    freopen("in1.txt","r",stdin);
    freopen("out1.txt","w",stdout);
    int T;
    cin >> T;
    for(int ts = 1; ts <= T; ts++)
    {
        memset(mem,0,sizeof(mem));
        cin >> N;
        for(int i = 1; i <= N; i++)
        {
            int a, b;
            cin >> a;
            for(int j = 0; j < a; j++)
            {
                cin >> b;
                mem[i][b] = 1;
            }
        }
        bool find = 0;
        for(int i = 1; i <= N && !find; i++)
        {
            for(int j = 1; j <= N && !find; j++)
            {
                if(i == j) continue;
                hit = 0;
                go(i, j);
                if(hit > 1) find = 1;
            }
        }
        cout << "Case #" << ts << ": ";
        if(find) cout << "Yes\n";
        else cout << "No\n";
    }
    return 0;
}
