#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<vector>
#include<algorithm>
#include<bitset>
#include<list>
#include<set>
#include<map>
#include<stack>
#include<queue>
#include<cmath>
#include<string>
#include<cstring>
#include<sstream>
#include<climits>

using namespace std;

typedef long long ll;
typedef vector<int> vi;
typedef pair<int, int> ii;
typedef vector<ii> vii;
typedef set<int> si;
typedef map<string, int> msi;

#define S(x) scanf("%d",&x)
#define SD(x) scanf("%lf",&x)
#define SL(x) scanf("%lld",&x)
#define pb(x) push_back(x)
#define mp make_pair
#define F(i, a, b) for (int i = int(a); i < int(b); i++)
#define forit(it, a) for (it = (a).begin(); it != (a).end(); it++)
#define M(x,i) memset(x,i,sizeof(x))

/* -------------------CODE GOES HERE---------------------- */

int dx[] = {1,0,-1,0};
int dy[] = {0,1,0,-1};

int main(){
	
	int T; S(T);
	int tst = 1;

	int R,C,N;
	int ans;

	int board[16][16];
	int x, count;
	bool flag;

	while(T--){

		M(board, 0);
		S(R); S(C); S(N);
		if(R > C) swap(R, C);
		ans = 0;

		if(N <= (((R*C)/2) + ((R*C) % 2))) ans = 0;
		else if(C == 1) ans = 0;

		else {

			F(i,0,R){
				F(j,0,C){

					if((i == 0 && j == 0) || ((i == 0) && (j == (C-1))) || (i == (R-1) && j == 0) || (i == (R-1) && j == (C-1)))	board[i][j] = min(2, R);
					else if((i == 0) || (j == 0) || (i == (R-1)) || (j == (C-1))) board[i][j] = min(3, R+1);
					else board[i][j] = 4;
				}
			}

			x = 4;
			count = 0;
			flag = false;

			while(x){

				F(i,0,R){
					F(j,0,C){
						if(count == ((R*C) - N)) flag = true;
						if(flag) break;
						if(board[i][j] == x){
							board[i][j] = -1;
							count++;

							F(k,0,4){
								if((i+dx[k] >= 0) && (i+dx[k] < R) && (j+dy[k] >= 0) && (j+dy[k] < C) && (board[i+dx[k]][j+dy[k]] != -1)) board[i+dx[k]][j+dy[k]] -= 1;
							}
						}
					}

					if(flag) break;
				}

				if(flag) break;
				x--;
			}

			F(i,0,R){
				F(j,0,C){
					if(!((i+j) % 2)) if(board[i][j] != -1) ans += board[i][j];
				}
			}
		}

		printf("Case #%d: %d\n", tst++, ans);
	}
}