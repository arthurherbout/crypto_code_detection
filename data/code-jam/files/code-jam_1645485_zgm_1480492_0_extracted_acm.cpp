#include<cstdio>
#include<cmath>
#include<cstring>
#include<time.h>
#include<vector>
#include<queue>
#include<list>
#include<stack>
#include<set>
#include<map>
#include<algorithm>
#include<iostream>
using namespace std;

struct Node{
    double s, p;
}L[51], R[51];

int main(){
    int T, n; char s[10];
    scanf("%d",&T);
    for(int cs=1; cs<=T; cs++){
        scanf("%d",&n);
        int ln = 0, rn = 0;
        for(int i=0; i<n; i++){
            scanf("%s",s);
            if( s[0]=='L' ){
                scanf("%lf%lf",&L[ln].s,&L[ln].p); ln++;
            }else{
                scanf("%lf%lf",&R[rn].s,&R[rn].p); rn++;
            }
        }
        printf("Case #%d: ",cs);
        if( n<=2 ){ printf("Possible\n"); continue; }
        
        bool done = false; double result = 0;
        while( true ){
            bool left = true; double time = 1e30; int a, b;
            bool ok = true;
            for(int i=0; i<ln; i++){
                for(int j=0; j<ln; j++){
                    if( i!=j && fabs(L[i].p-L[j].p)<1e-6 && fabs(L[i].s-L[j].s)<1e-6 ){
                        time = 0;
                    }
                    if( L[i].p>=L[j].p-1e-6 ){
                        if( L[i].s<=L[j].s+1e-6 ) ok = false;
                        if( L[i].s<L[j].s ){
                            double t = (L[i].p-L[j].p)/(L[j].s-L[i].s);
                            if( time>t ){ time = t; a = i; b = j; }
                        }
                    }
                }
            }
            for(int i=0; i<rn; i++){
                for(int j=0; j<rn; j++){
                    if( i!=j && fabs(R[i].p-R[j].p)<1e-6 && fabs(R[i].s-R[j].s)<1e-6 ){
                        time = 0;
                    }
                    if( R[i].p>=R[j].p-1e-6 ){
                        if( R[i].s<=R[j].s+1e-6 ) ok = false;
                        if( R[i].s<R[j].s ){
                            double t = (R[i].p-R[j].p)/(R[j].s-R[i].s);
                            if( time>t ){ left=false; time = t; a = i; b = j; }
                        }
                    }
                }
            }//printf("%.6f\n",time);
            if( time==0 ) break;
            if( ok || time>1e20 ){ done = true; break; }
            
            if( fabs(result-time)<1e-6 || result>time ) break;
            result = time;
            if( left ){
                R[rn] = L[a]; rn++;
                swap( L[a], L[ln-1] ); ln--;
            }else{
                L[ln] = R[a]; ln++;
                swap( R[a], R[rn-1] ); rn--;
            }
        }
        if( done ) printf("Possible\n");
        else printf("%.6f\n",result);
    }
    return 0;
}

/*struct Node{
    int a, b, num;
}p[1000];
inline bool cmp( Node s, Node t ){
    return s.b<t.b || ( s.b==t.b && s.a>t.a );
}

int main(){
    int T, n;
    scanf("%d",&T);
    for(int cs=1; cs<=T; cs++){
        scanf("%d",&n);
        for(int i=0; i<n; i++){
            scanf("%d%d",&p[i].a,&p[i].b);
            p[i].num = 2;
        }
        sort( p, p+n, cmp );
        int star = 0, time = 0;
        for(int i=0,j; i<n; i++){
            if( p[i].b<=star ){
                star += p[i].num;
                time++;
            }else{
                for(j=n-1; j>=i; j--){
                    if( p[j].num==2 && p[j].a<=star ){
                        p[j].num = 1;
                        star += 1;
                        time++;
                        break;
                    }
                }
                if( j==i-1 ){ time = -1; break; }
                i--;
            }
        }
        printf("Case #%d: ",cs);
        if( time==-1 ) printf("Too Bad\n");
        else printf("%d\n",time);
    }
    return 0;
}*/

/*int main(){
    int T, A, B; double p[100000];
    scanf("%d",&T);
    for(int cs=1; cs<=T; cs++){
        scanf("%d%d",&A,&B);
        for(int i=0; i<A; i++) scanf("%lf",&p[i]);
        double exp = B+2, pp = 1;
        for(int i=0; i<A; i++){
            double s = (A-i+B-i+1)*pp+(A-i+B-i+1+B+1)*(1-pp);
            //printf("%.6f\n",s);
            exp = min( exp, s );
            pp *= p[i];
        }
        //printf("%.6f\n",(B-A+1)*pp+(B-A+1+B+1)*(1-pp));
        exp = min( exp, (B-A+1)*pp+(B-A+1+B+1)*(1-pp) );
        printf("Case #%d: %.6f\n",cs,exp);
    }
    return 0;
}*/
