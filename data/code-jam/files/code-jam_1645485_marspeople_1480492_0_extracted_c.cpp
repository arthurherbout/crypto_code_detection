#include <cstdio>
#include <cstring>
#include <cctype>
#include <cmath>
#include <stack>
#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include <queue>
using namespace std;
#define INF 0x3f3f3f3f
#define REP(i,n) for(int i=0; i<(n); i++)

int cmp(double a, double b=0) {
    if (fabs(a-b)<1e-8) return 0;
    if (a<b) return -1;
    return 1;
}

double v[100],p[100];
int n, m[100];

void updm(double t) {
    REP(i,n) {
        bool deu=true;
        REP(j,n) if (i!=j) {
            double pi=p[i]+v[i]*t;
            double pj=p[j]+v[j]*t;
            
            if (cmp(pj,pi-5)>0 && cmp(pj,pi+5)<0) deu=false;
        }
        if (deu) m[i]=3;
    }
}

int main() {
    int nt;

    scanf(" %d",&nt);
    for (int ct=1;ct<=nt;ct++) {
        scanf("%d",&n);
        
        REP(i,n) {
            char tr;
            scanf(" %c%lf%lf",&tr,&v[i],&p[i]);
            if (tr=='L') m[i]=1;
            else m[i]=2;
        }

        updm(0);

        double t=0, res;
        bool deu=true;
        while (1) {
            double rt=1e100;
            REP(i,n)
                REP(j,n) 
                if (v[i]>v[j] && cmp(p[i]+v[i]*t,p[j]+v[j]*t-5)<0) {
                    double ct=(p[j]-p[i]-5)/(v[i]-v[j]);
                    rt=min(rt,ct);
                }
                else if (v[i]>v[j] && cmp(p[i]+v[i]*t,p[j]+v[j]*t+5)<0) {
                    double ct=(p[j]-p[i]+5)/(v[i]-v[j]);
                    rt=min(rt,ct);
                }

            if (!cmp(rt,1e100)) break;

            updm(rt);

            //printf("At %lf\n",rt);
            //REP(i,n)
                //printf("car %d at %lf %d\n",i,p[i]+v[i]*rt,m[i]);
            //puts(" ");
            
            REP(i,n)
                REP(j,n)
                    if (v[i]>v[j] && cmp(p[i]+v[i]*t,p[j]+v[j]*t-5)<0
                        && !cmp((p[j]-p[i]-5)/(v[i]-v[j]),rt)) {
                        REP(k,n)
                            if (k!=i && k!=j) {
                                double pk=p[k]+v[k]*rt;
                                double pi=p[i]+v[i]*rt;
                                double pj=p[j]+v[j]*rt;
                                
                                if (cmp(pk,pi)>=0 && cmp(pk,pi+5)<=0) {
                                    deu=false;
                                    res=rt;
                                }
                                else if (cmp(pk,pi-5)>0 && cmp(pk,pi)<0) {
                                    if ((m[k]&1) && (m[i]&2) && (m[j]&1))
                                        m[k]=1, m[i]=2, m[j]=1;
                                    else if ((m[k]&2) && (m[i]&1) && (m[j]&2))
                                        m[k]=2, m[i]=1, m[j]=2;
                                    else
                                          deu=false, res=rt;
                                }
                                else if (cmp(pk,pj)>0 && cmp(pk,pj+5)<0) {
                                    if ((m[i]&1) && (m[j]&2) && (m[k]&1))
                                        m[i]=1, m[j]=2, m[k]=1;
                                    else if ((m[i]&2) && (m[j]&1) && (m[k]&2))
                                        m[i]=2, m[j]=1, m[k]=2;
                                    else
                                          deu=false, res=rt;
                                }
                            }
                        }
            if (!deu) break;
            t=rt;
        }
        printf("Case #%d: ",ct);
        
        if (deu) printf("Possible\n");
        else printf("%.6lf\n",res);
    }
    return 0;
}
