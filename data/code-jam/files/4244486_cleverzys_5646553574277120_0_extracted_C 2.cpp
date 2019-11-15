#include <cstdio>
#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <deque>
#include <set>
#include <map>
#include <list>
#include <limits>
#include <queue>
#include <stdexcept>
#include <iomanip> 
#include <sstream>
using namespace std;

////////////////////////////////////////////////////
#define TRY
#define SMALL
//#define LARGE
/////////////////////////////////////////////////////
#define INF 100000000
typedef pair<int,int> PII;
#define MP make_pair
#define MAX_N 1000002
////////////////////////////////////////////////////////
void Solve();


void main() 
{
#ifdef TRY
	freopen("1.txt", "rt", stdin);
	//freopen("2.out","wt",stdout);
#endif
#ifdef SMALL
	freopen("C-small-attempt1.in","rt",stdin);
	freopen("C-small.txt","wt",stdout);
#endif
#ifdef LARGE
	freopen("C-large.in","rt",stdin);
	freopen("C-large.txt","wt",stdout);
#endif

	int Numcase;
	cin>>Numcase;

	for(int test=1;test<=Numcase;test++)
	{
		cout<<"Case #"<<test<<": ";
		//cout<<"Case #"<<test<<":"<<endl;
		Solve();
	}
}
void Solve(){
	int C,D,V;
	int d[101];
	cin>>C>>D>>V;
	for(int i=0;i<D;i++){
		cin>>d[i];
	}
	int sum=0;
	int j=0;
	int ans=0;
	int basenum=1;

	while(j<D){
		if(d[j]-sum==1){
			sum+=d[j];
			j++;
		}
		else if(d[j]>sum){
			while(sum<d[j]-1){
				ans++;
				basenum=(sum+1);
				sum+=basenum;
			}
			sum+=d[j];
			if(sum>=V) break;
			j++;
		}
		else{
			sum+=d[j];
			j++;
		}
	}
	if(sum>=V) {cout<<ans<<endl;return;}
	while(sum<V){
		ans++;
		sum=sum*2+1;
	}
	cout<<ans<<endl;
	
}