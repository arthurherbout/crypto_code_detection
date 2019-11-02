//poj 

#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<algorithm>
#include<map>
#include<math.h>

using namespace std;
typedef long long llt;

#define maxv 300

#define LEN 12
#define myAbs(x) ((x)>=0?(x):-(x))
#define Max(x,y) ((x)>(y)?(x):(y))
#define Min(x,y) ((x)<(y)?(x):(y))
#define inf 1999999999
#define MOD 200039
#define eps 1e-8
#define EPS 1e-8
#define M_PI 3.14159265358979323846


typedef struct nodE{
	double p;
	char str[LEN];
	int l, r;
}node;
node nd[maxv];
int end;
int ttt;
int n;
char str[maxv];
int slen;
char s[maxv][LEN];
int len;
//map<string , int> M;
//map<string , int>::iterator it;


void dfs( int l , int r, int pp){
	int i,j,k;
	int ll,rr;
	int cnt;
	int flag =0 ;
	for(i=l;i<r ; ++ i){
		if(str[i]=='('){
			flag =1 ;
			ll = i+1;
			break;
		}
	}
	if(!flag){
		sscanf(str+l , "%lf", &nd[pp].p);
		nd[pp].l =  nd[pp].r = -1;
	}else{
		sscanf(str+l , "%lf %s", &nd[pp].p, nd[pp].str);
		cnt = 1;
		for(i=ll;i< r;++i){
			if(str[i]=='(')
				++cnt;
			if(str[i]==')'){
				--cnt;
				if(cnt==0){
					rr= i;
					break;
				}
			}
		}
		nd[pp].l = end;
		dfs(ll, rr, end++);
		for(i=rr+1;i< r;++i){
			if(str[i]=='('){
				ll= i+1;
				break;
			}
		}
		for(i= r-1;i>=l;--i){
			if(str[i]==')'){
				rr=i;
				break;
			}
		}
		nd[pp].r= end;
		dfs(ll, rr, end++);
	}
}
double dfs1(int pp){
	int i,j,k;
	int flag;
	if(nd[pp].l==-1){
		return nd[pp].p;
	}
	flag =0 ;
	for(i=0;i<len;++i){
		if(strcmp(s[i], nd[pp].str)==0){
			flag = 1; 
			break;
		}
	}
	if(flag){
		return nd[pp].p * dfs1(nd[pp].l);
	}else{
		return nd[pp].p * dfs1(nd[pp].r);
	}
}
void solve(){
	int i,j,k;
	int l,r;
	double res;
	scanf("%d",&n);
	while(getchar()!='\n');
	end=1;
	
	//M.clear();
	slen=0;
	for(i=0;i<n;++i){
		gets(str+slen);
		slen=strlen(str);
	}
	//printf("%d\n%s\n", slen, str);
	for(i=0;i<slen;++i){
		if(str[i]=='(')
			break;
	}
	l=i;
	for(i=slen-1;i>=0;--i){
		if(str[i]==')')
			break;
	}
	r= i;
	dfs(l+1, r, 0);
	
	scanf("%d", &n);
	
	printf("Case #%d:\n", ++ttt);
	for(i=0;i<n;++i){
		scanf("%s",str);
		scanf("%d", &len);
		for(j=0;j<len;++j){
			scanf("%s", s[j]);
		}
		res=dfs1(0);
		printf("%.7lf\n",res);
	}
	//printf("%d\n", end);
	
}
int main(){
	//freopen("out.txt","w",stdout);
	int t;
	scanf("%d", &t);
	ttt=0;
	while(--t>=0)
		solve();
	//system("PAUSE");
	return 0;
}



/*
*/
