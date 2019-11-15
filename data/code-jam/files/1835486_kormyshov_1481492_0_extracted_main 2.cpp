#include <iostream>
#include <cstdio>
#include <memory.h>
#include <cstring>
#include <cmath>

#include <vector>
#include <deque>
#include <queue>
#include <stack>
#include <algorithm>

#define ABS(a) ((a)<0?-(a):(a))
#define SIGN(a) ((a)<0?-1:((a)>0?1:0))
#define SQR(a) ((a)*(a))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define MIN(a,b) (((a)<(b))?(a):(b))

#define REP(i, n) for(int i=0; i<(n); ++i)
#define FOR(i, a, b) for(int i=(a); i<(b); ++i)

#define in ({int x;scanf("%d", &x);x;})

#define PI   (3.1415926)
#define INF  (2147483647)
#define INF2 (1073741823)
#define EPS  (0.000001)
#define y1 stupid_cmath

typedef long long LL;

using namespace std;

void run(){
    int M, F, N;
    cin>>M>>F>>N;
    int i, mas[2000100], a, b, m[250];
    memset(mas, 127, sizeof(mas));
    memset(m, 0, sizeof(m));
    for(i=0;i<N;++i){
        cin>>a>>b;
        mas[b]=MIN(mas[b], a);
        m[i]=b;
    }
    for(i=2000000;i>=0;--i) mas[i]=MIN(mas[i], mas[i+1]);
    for(i=1;i<=2000000;++i){
        mas[i]+=mas[i-1];
        if(mas[i]>2000000) mas[i]=3000000;
    }
    for(i=0;i<=2000000;++i){
        mas[i]+=F;
    }
    for(i=0;i<N-1;++i)
        if(mas[m[i]]/(m[i]+1.0) > mas[m[i+1]]/(m[i+1]+1.0) ||
           (mas[m[i]]/(m[i]+1.0) == mas[m[i+1]]/(m[i+1]+1.0) && m[i]<m[i+1])){
            a=m[i];
            m[i]=m[i+1];
            m[i+1]=a;
            if(i) i-=2;
        }
    int r=0;
/*
    cout<<"mas = ";
    for(i=0;i<10;++i) cout<<mas[i]<<" ";
    cout<<endl;

    cout<<"m = ";
    for(i=0;i<10;++i) cout<<m[i]<<" ";
    cout<<endl;
*/
    while(M){
        for(i=0;i<N;++i) if(mas[m[i]]<=M) break;
        if(i==N) break;
        M-=mas[m[i]];
        r+=m[i]+1;
    }
    if(M>=mas[0]){
        for(i=0;M>=mas[i];++i);
        r+=i;
    }
    cout<<r<<endl;
}

int main(){
	freopen("input.txt","r",stdin); freopen("output.txt","w",stdout);
    int T;
    cin>>T;
    for(int ii=1;ii<=T;++ii){
        cout<<"Case #"<<ii<<": ";
//        printf("Case #%d: ",ii);
        run();
    }
	return 0;
}
