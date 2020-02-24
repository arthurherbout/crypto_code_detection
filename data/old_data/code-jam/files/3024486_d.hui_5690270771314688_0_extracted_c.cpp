#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<cmath>
#include<iostream>
#include<fstream>
#include<map>
#include<set>
#include<queue>
#include<vector>
#include<algorithm>
using namespace std;
int T,n,x,o;
char c;
int st[3000];
int un[5];
int cv[5];
bool f;
int main()
{
	freopen("c-small.in","r",stdin);
	freopen("c-small.out","w",stdout);
	scanf("%d",&T);
	for(int zz=1;zz<=T;zz++)
    {
        printf("Case #%d: ",zz);
        f=false;
        o=0;
        memset(st,0,sizeof(st));
        memset(un,0,sizeof(un));
        memset(cv,0,sizeof(cv));
        scanf("%d",&n);
        while(n--)
        {
            scanf("\n%c %d",&c,&x);
            //cout<<c<<x<<endl;
            if(c=='E')
            {
                if(x==0)
                {
                    un[1]++;
                    if(cv[2]==0)
                        cv[1]++;
                    else
                        cv[2]--;
                }
                else
                {
                    if(st[x]==2 || st[x]==0)
                        st[x]=1;
                    else if(un[2]>0)
                        un[2]--;
                    else
                        f=true;
                }
            }
            if(c=='L')
            {
                if(x==0)
                {
                    un[2]++;
                    cv[2]++;
                }
                else
                {
                    if (st[x]==1 || st[x]==0)
                    {
                        if(cv[1]>0 && st[x]==0)
                            cv[1]--;
                        st[x]=2;
                    }
                    else if(un[1]>0)
                    {
                        if(cv[1]>0)
                            cv[1]--;
                        un[1]--;
                    }
                    else
                        f=true;
                }
            }
        }
        o=0;
        for(int i=1;i<=2000;i++)
            if(st[i]==1)
                o++;
        o+=cv[1];
        if(f)
            printf("CRIME TIME\n");
        else
            printf("%d\n",o);
    }
	fclose(stdin);
	fclose(stdout);
	return 0;
}
