#include	<cstdio>
#include	<cstdlib>
#include	<cstring>
#include	<string>
#include	<vector>
#include	<cmath>
#include	<algorithm>
#include	<cassert>
#include	<set>
#include	<map>
#include	<queue>
#include	<iostream>
#include <fstream>
using namespace std;
#define pb push_back
#define REP(i,n) for(int i=0;i<(n);i++ ) 

typedef long long LL;
typedef pair<int,int> piii;

const int maxn=20;

int hp[maxn][maxn];
int xd[maxn][maxn];
int jg[maxn][maxn];

int C,R,c,r;

void getxd()
{
        memset(xd,0,sizeof xd);
        REP(i,R+1)
                REP(j,C+1)
                {
                        if (!i || !j)
                                continue;
                        int big=-1,dir=0;
                        if (hp[i+1][j]>big)
                        {
                                big=hp[i+1][j];
                                dir=1;
                        }
                        if (hp[i][j-1]>big)
                        {
                                big=hp[i][j-1];
                                dir=3;
                        }
                        if (hp[i][j+1]>big)
                        {
                                big=hp[i][j+1];
                                dir=4;
                        }
                        if (hp[i-1][j]>big)
                        {
                                big=hp[i-1][j];
                                dir=2;
                        }
                        xd[i][j]=dir;
                }
}

void getjg()
{
        REP(i,R+2)
                REP(j,C+2)
                        jg[i][j]=hp[i][j];
        REP(i,R+1)
                REP(j,C+1)
                {
                        if (hp[i][j]<=0)
                                continue;
                        if (xd[i][j]==1)
                                jg[i+1][j]-=hp[i][j];
                        if (xd[i][j]==2)
                                jg[i-1][j]-=hp[i][j];
                        if (xd[i][j]==3)
                                jg[i][j-1]-=hp[i][j];
                        if (xd[i][j]==4)
                                jg[i][j+1]-=hp[i][j];
                }
}

int P=7;
int P2=78125;


int main()
{
        int caseNumber;
        cin>>caseNumber;
        REP(caseIndex, caseNumber)
        {
                cin>>C>>R>>c>>r;
                r=R+1-r;
                memset(hp,-1,sizeof hp);
                REP(i,R)
                        REP(j,C)
                                cin>>hp[i+1][j+1];
                int best=0;
                REP(sol,P2)
                {
                        int res=10000000,day=0;
                        int PP=sol;
                        REP(d,P)
                        {
                                day++;
                                getxd();
                                xd[r][c]=(PP%5)+1;PP/=5;
                                getjg();
                                REP(i,R+1)
                                {
                                        REP(j,C+1)
                                        {
                                                hp[i][j]=jg[i][j];
                                                //cout<<hp[i][j]<<' ';
                                        }
                                        //cout<<endl;
                                }
                                //cout<<hp[r][c]<<endl;
                                if (hp[r][c]<=0)
                                {
                                        res=day;
                                        break;      
                                }
                                if (hp[r][c+1]<=0 && hp[r-1][c]<=0 && hp[r+1][c]<=0 && hp[r][c-1]<=0)
                                        break;
                        }
                        best=max(best,res);
                }
                printf("Case #%d: ", caseIndex+1);
                if (best==10000000)
                        puts("forever");
                else
                        printf("%d day(s)\n",best);
        }
}
