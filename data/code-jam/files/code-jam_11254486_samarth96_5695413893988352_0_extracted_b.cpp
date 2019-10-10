#include<bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef unsigned long long ull;
typedef pair<int, int> pii;
typedef vector<int> vi;
typedef vector<pii> vii;
#define mp make_pair
#define pb push_back
#define ft first
#define sd second

#define mod 1000000007
#define inf 2000000001

int main()
{
    freopen("B-small-attempt1.in","r",stdin);
    int qq,tt;
    scanf("%d",&tt);
    char ans1[tt][4],ans2[tt][4];
    char all[1000][4];
    int i,j,k,p,n;
    p=0;
    for (i=0;i<10;i++)
    {
        for (j=0;j<10;j++)
        {
            for (k=0;k<10;k++)
            {
                all[p][0]=i+'0';
                all[p][1]=j+'0';
                all[p][2]=k+'0';
                all[p][3]='\0';
                p++;
            }
        }
    }
    for (qq=1;qq<=tt;qq++)
    {
        vi a,b;
        char c[5],d[5];
        scanf("%s",c);
        scanf("%s",d);
        int z,l;
        l=strlen(c);
        if (strlen(c)==1)
            z=10;
        else if (strlen(c)==2)
            z=100;
        else
            z=1000;
    for (i=0;i<z;i++)
        {
            bool ok=true;
            for (j=(3-l);j<3;j++)
            {
                if (c[j+(l-3)]=='?')
                    continue;
                else if (c[j+(l-3)]!=all[i][j])
                {
                    ok=false;
                    break;
                }
            }
            if (ok)
            {
                n=0;
                p=1;
                for (k=2;k>=(3-l);k--)
                {
                    n+=((all[i][k]-'0')*p);
                    p*=10;
                }
                a.pb(n);
            }
        }
        l=strlen(d);
        if (strlen(d)==1)
            z=10;
        else if (strlen(d)==2)
            z=100;
        else
            z=1000;
        for (i=0;i<z;i++)
        {
            bool ok=true;
            for (j=(3-l);j<3;j++)
            {
                if (d[j+(l-3)]=='?')
                    continue;
                else if (d[j+(l-3)]!=all[i][j])
                {
                    ok=false;
                    break;
                }
            }
            if (ok)
            {
                n=0; p=1;
                for (k=2;k>=(3-l);k--)
                {
                    n+=((all[i][k]-'0')*p);
                    p*=10;
                }
                b.pb(n);
            }
        }
        sort(a.begin(), a.end());
        sort(b.begin(), b.end());
        int s=inf;
        int x,y;
        for (i=0;i<a.size();i++)
        {
            for (j=0;j<b.size();j++)
            {
                if (abs(a[i]-b[j])<s)
                {
                    x=a[i];
                    y=b[j];
                    s=abs(a[i]-b[j]);
                }
            }
        }
        n=x;
        z=0;
        while(n>0)
        {
            z++;
            n/=10;
        }
        i=0;
        if (strlen(c)>z)
        {
            for (i=0;i<(strlen(c)-z);i++)
                ans1[qq][i]='0';
        }
        for (j=strlen(c)-1;j>=i;j--)
        {
            ans1[qq][j]=(x%10)+'0';
            x/=10;
        }
        ans1[qq][strlen(c)]='\0';
        n=y;
        z=0;
        while(n>0)
        {
            z++;
            n/=10;
        }
        i=0;
        if (strlen(d)>z)
        {
            for (i=0;i<(strlen(d)-z);i++)
                ans2[qq][i]='0';
        }
        for (j=strlen(d)-1;j>=i;j--)
        {
            ans2[qq][j]=(y%10)+'0';
            y/=10;
        }
        ans2[qq][strlen(d)]='\0';
    }
    fclose(stdin);
    freopen("out.txt","w",stdout);
    for (qq=1;qq<=tt;qq++)
    {
        printf("Case #%d: %s %s\n",qq,ans1[qq],ans2[qq]);
        fflush(stdout);
    }
    fclose(stdout);
}
