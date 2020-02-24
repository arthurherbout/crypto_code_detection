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
#define pb(x) push_back(x)
#define mp make_pair
#define F(i, a, b) for (int i = int(a); i < int(b); i++)
#define forit(it, a) for (it = (a).begin(); it != (a).end(); it++)
#define M(x,i) memset(x,i,sizeof(x))

/* -------------------CODE GOES HERE---------------------- */


int main(){
	
	int T,R,C; S(T);
	int tst = 1, ans;

	string row;
	char board[100][100];
	bool flag, up_flag, down_flag, left_flag, right_flag;

	while(T--){

		M(board, '.');
		flag = true;

		ans = 0;

		S(R); S(C);
		getline(cin, row);

		F(i,0,R){
			getline(cin, row);

			F(j,0,C){
				board[i][j] = row[j];
			}
		}

		F(i,0,R){
			F(j,0,C){

				if(board[i][j] != '.'){
					up_flag = down_flag = left_flag = right_flag = false;
					
					F(a,0,i){
						if(board[a][j] != '.'){
							up_flag = true;
							break;
						}
					}
						
					F(a,i+1,R){
						if(board[a][j] != '.'){
							down_flag = true;
							break;
						}
					}

					F(a,0,j){
						if(board[i][a] != '.'){
							left_flag = true;
							break;
						}
					}

					F(a,j+1,C){
						if(board[i][a] != '.'){
							right_flag = true;
							break;
						}
					}

					if(!(up_flag || down_flag || right_flag || left_flag)){
						flag = false;
						break;
					}

					else {
						if(board[i][j] == '^' && (!up_flag)) ans++;
						if(board[i][j] == 'v' && (!down_flag)) ans++;
						if(board[i][j] == '<' && (!left_flag)) ans++;
						if(board[i][j] == '>' && (!right_flag)) ans++;
					}
				}
			}
			if(!flag) break;
		}

		if(!flag) printf("Case #%d: IMPOSSIBLE\n", tst++);
		else printf("Case #%d: %d\n", tst++, ans);
	}	
}