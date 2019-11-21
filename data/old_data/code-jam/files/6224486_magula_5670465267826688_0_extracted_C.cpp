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

struct q{
	int s;
	q(){s=1;}
	q(int _s){s=_s;}
};

//1->1 2->i 3->j 4->k
int MT[][4]={	{1,2,3,4},
		{2,-1,4,-3},
		{3,-4,-1,2},
		{4,3,-2,-1}};
int Y[1000];
int sgn(int s){	return s>0?+1:-1;}
q operator*(q a, q b){
	return q(sgn(a.s*b.s)*MT[abs(a.s)-1][abs(b.s)-1]);
}
bool operator==(q a, q b){
	return a.s==b.s;
}

#define Result(a) {printf("Case #%d: %s\n",tc+1,a?"YES":"NO");continue;}

q A[20000];
q H[20000];

int main(){
	Y['1']=1;
	Y['i']=2;
	Y['j']=3;
	Y['k']=4;
	
	int TC;scanf("%d ",&TC);
	RE(tc,TC){
		int L,X;scanf("%d %d ",&L,&X);
		RE(i,L){
			char c;scanf("%c ",&c);
			A[i]=q(Y[c]);
		}
		if(L*X<3)
			Result(0);
		int I=INF,J=-INF;
		q a,b,c;
		RE(i,min(X,20))
		RE(j,L){
			a=a*A[j];
			if(a==q(2))//=i
				I=min(I,i*L+j);
		}
		RE(i,min(X,20))
		BREP(j,L,0){
			b=A[j]*b;
			if(b==q(4))//=k
				J=max(J,(X-1-i)*L+j);
		}
		bool ok=false;
		RE(i,min(X,20)){
			H[i]=c;
			RE(j,L)
				c=c*A[j];
			if(c==q(-1)){//=-1
				ok=H[X%(i+1)]==q((X/(i+1))%2==0?-1:1);
				break;
			}
		}
		Result(
			ok&&I+1<J
		);
	}
	return 0;
}