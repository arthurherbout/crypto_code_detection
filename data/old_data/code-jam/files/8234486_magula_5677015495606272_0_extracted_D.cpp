#include <cstdio>
#include <string>
#include <algorithm>
#include <cmath>
#include <vector>
#include <set>
#include <queue>
#include <map>
#include <stack>
#include <cassert>
using namespace std;

typedef long long LL;
typedef vector<int> vi;
typedef pair<int, int> ii;
typedef vector<ii > vii;
typedef queue<int> qi;
typedef set<int> si;

#define RE(i,b) for(int i=0; i<(int)(b); i++)
#define REP(i,a,b) for(int i=(a); i<(int)(b); i++)
#define BREP(i,a,b) for(int i=(a)-1; i>=(b); i--)
#define FE(i,X) for(typeof((X).begin()) i=(X).begin(); i!=(X).end(); ++i)

#define INF 1000000000
#define MP make_pair
#define FI first
#define SE second

bool D[1<<12];
int dp[8][1<<12][1<<6];

int main(){
	int TC;scanf("%d ",&TC);
	RE(tc,TC){
		printf("Case #%d: ",tc+1);
		int R,C;
		scanf("%d %d ",&R,&C);
		int P=1;RE(r,C)P*=3;
		int Q=1;RE(r,C)Q*=2;
		fill_n(D,1<<12,0);
		RE(i,8)RE(j,1<<12)RE(k,1<<6)
		dp[i][j][k]=0;
		RE(r,R-1){
			RE(bi,P)RE(hi,Q){
				int A[C];
				int t=bi;
				RE(c,C){
					A[c]=(t%3);
					t/=3;
				}
				if(r==0){
					if(hi!=0)
						break;
					bool ok=true;
					RE(c,C){
						int ci=0;
						BREP(c2,C,0){
							ci*=3;
							ci+=A[(c+c2)%C];
						}
						if(D[ci]){
							ok=false;
							break;
						}
					}
					D[bi]=true;
					if(!ok)
						continue;
					dp[0][bi][0]=1;
				}
				if(dp[r][bi][hi]!=0){
					RE(ci,P){
						int A2[C];
						t=ci;
						RE(c,C){
							A2[c]=(t%3);
							t/=3;
						}
						bool fail=false,thisonenot=false;
						int gi=0;
						RE(c,C){
							bool davor=(1<<c)&hi;
							int Uebrig=A[c]+1-davor-(A[c]==A[(c+C-1)%C])-(A[c]==A[(c+C+1)%C]);
							if(Uebrig<0||Uebrig>1){
								fail=true;
								break;
							}
							if((A2[c]==A[c])?(!Uebrig):(Uebrig)){
								thisonenot=true;
								break;
							}
							if(A2[c]==A[c])
								gi|=1<<c;
						}
						if(fail)
							break;
						if(thisonenot)
							continue;
						dp[r+1][ci][gi]=(dp[r+1][ci][gi]+dp[r][bi][hi])%1000000007;
					}
				}
			}
		}
		int Erg=0;
		RE(bi,P)RE(hi,Q){
			int A[C];
			int t=bi;
			RE(c,C){
				A[c]=(t%3);
				t/=3;
			}
			bool ok=true;
			RE(c,C)
				if(A[c]+1!=(A[c]==A[(c+C-1)%C])+(A[c]==A[(c+C+1)%C])+(bool)(hi&(1<<c))){
					ok=false;
					break;
				}
			if(ok){
				Erg+=dp[R-1][bi][hi];
// 				if(dp[R][bi][hi]){
// 					RE(c,C)
// 						printf("%d ",(bool)((1<<c)&hi));
// 						printf("\n");
// 					RE(c,C)
// 						printf("%d ",A[c]+1);
// 					printf("+%d\n",dp[R][bi][hi]);
// 				}
			}
		}
		printf("%d\n",Erg);
	}
	return 0;
}