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

const double eps=0.000001;

int main(){
	int TC;scanf("%d ",&TC);
	RE(tc,TC){
		printf("Case #%d: ",tc+1);
		int N;double V,XX;scanf("%d %lf %lf ",&N,&V,&XX);
		double R[2],X[2];
		RE(i,N)	scanf("%lf %lf ",&R[i],&X[i]);
		if(N==1){
			if(abs(X[0]-XX)<eps){
				printf("%.9lf\n",V/(R[0]));
			}else{
				printf("IMPOSSIBLE\n");
			}
		}
		else if(((X[0]<XX&&X[1]<XX)||(X[0]>XX&&X[1]>XX))&&abs(X[0]-XX)>eps&&abs(X[1]-XX)>eps){
			printf("IMPOSSIBLE\n");
		}
		else if(abs(X[0]-X[1])<eps){
			if(abs(X[0]-XX)<eps){
				printf("%.9lf\n",V/(R[0]+R[1]));
			}else{
				printf("IMPOSSIBLE\n");
			}
		}else{
			printf("%.9lf\n",V*max((XX-X[0])/(X[1]-X[0])/R[1],(XX-X[1])/(X[0]-X[1])/R[0]));
		}
	}
	return 0;
}