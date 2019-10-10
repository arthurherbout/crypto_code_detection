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
#define N 2000

int main()
{
    freopen("C-small-attempt0.in","r",stdin);
    int qq,tt;
    scanf("%d",&tt);
    int ans[tt];
    for (qq=1;qq<=tt;qq++)
    {
        int i,n,x,y;
        scanf("%d",&n);
        map<string, int> m1,m2;
        char a[29],b[29];
        for (i=0;i<n;i++)
        {
            scanf("%s%s",a,b);
            m1[a]++;
            m2[b]++;
        }
        map<string, int>::iterator k;
        x=0;
        for (k=m1.begin();k!=m1.end();k++)
            x=max(x, k->sd);
        y=0;
        for (k=m2.begin();k!=m2.end();k++)
            y=max(y, k->sd);
        ans[qq]=min(x-1, y-1);
    }
    fclose(stdin);
    freopen("out.txt","w",stdout);
    for (qq=1;qq<=tt;qq++)
    {
        printf("Case #%d: %d\n",qq,ans[qq]);
        fflush(stdout);
    }
    fclose(stdout);
}
