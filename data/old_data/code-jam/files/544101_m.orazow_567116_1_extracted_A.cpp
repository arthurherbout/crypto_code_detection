#include<iostream>
#include<cstdio>
#include<cstring>
#include<string>
#include<vector>
#include<algorithm>
using namespace std;

#define FOR(i, a, b) for (int i(a), _b(b); i <= _b; i++)
#define FORD(i, a, b) for (int i(a), _b(b); i >= _b; i--)
#define REP(i, n) for (int i(0), _n(n); i < _n; i++)
#define REPD(i, n) for (int i((n)-1); i >= 0; i--)
#define CLR(x, with) memset((x), with, sizeof(x))
#define ALL(x) (x).begin(), (x).end()

typedef long long LL;
typedef unsigned long long ULL;

int N, K;
char board[101][101];
int was[101][101];

const int dx[] = {1,  0,  1,  1};// 0, -1, 1,  1, -1};
const int dy[] = {0,  1,  1, -1};//, 1,  1, 1, -1, -1};

bool recur(int x, int y, char c, int d, int cnt)
{
    was[x][y] = 1;
    //cout <<"x = "<<x<<" "<<"y = "<<y<<" c = "<<c<<endl;
    if (cnt >= K) return true;
    int xx = x + dx[d];
    int yy = y + dy[d];
    if (xx < 0 || xx >= N) return false;
    if (yy < 0 || yy >= N) return false;
    if (was[xx][yy] || board[xx][yy] != c) return false;
    return recur(xx, yy, c, d, cnt+1);
}

int main() {
    int T;
    cin >> T;
    FOR(t, 1, T)
    {
        cin >> N >> K;
        string s;
        bool Rsol, Bsol;
        REP(i, N)
        {
            cin >> s; //cout << s << endl;
            int j = N;
            REP(k, N) board[i][k]='.';
            REPD(k, N) if (s[k] != '.') board[i][--j] = s[k];
            //REPD(k, j+1) board[i][j]='.';
        }

        bool res = false;
        CLR(was, 0);
        REP(i, N)
        {
            REP(j, N)
                if (board[i][j] == 'R')
                {
                    res |= recur(i, j, 'R', 0, 1); CLR(was, 0);
                    res |= recur(i, j, 'R', 1, 1); CLR(was, 0);
                    res |= recur(i, j, 'R', 2, 1); CLR(was, 0);
                    res |= recur(i, j, 'R', 3, 1); //CLR(was, 0);
                    if (res) break;
                }
            if (res) break;
        }
        Rsol = res;

        res = false;
        CLR(was, 0);
        REP(i, N)
        {
            REP(j, N)
                if (board[i][j] == 'B')
                {
                    res |= recur(i, j, 'B', 0, 1); CLR(was, 0);
                    res |= recur(i, j, 'B', 1, 1); CLR(was, 0);
                    res |= recur(i, j, 'B', 2, 1); CLR(was, 0);
                    res |= recur(i, j, 'B', 3, 1); CLR(was, 0);
                    if (res) break;
                }
            if (res) break;
        }
        Bsol = res;

        //cout << Rsol<<" "<<Bsol<<endl;
        /*
        REP(i, N)
        {
            REP(j, N)
            cout<<board[i][j]<<" "; cout<<endl;
        }
        */
        printf("Case #%d: ", t);
        if (Rsol && Bsol) printf("Both\n");
        else if (Rsol) printf("Red\n");
        else if (Bsol) printf("Blue\n");
        else printf("Neither\n");
    }

    return 0;
}
