/*
    Rezwan_4029
    AUST , CSE-25
*/

#include<stdio.h>
#include<iostream>
#include<cstring>
#include<string>
#include<cctype>
#include<vector>
#include<algorithm>
#include<cmath>
#include<map>
#include<sstream>
#include<set>
#include<queue>
#define pb push_back
#define INF 1<<30
#define ms(a,v) memset(a,v,sizeof a)
#define II ({int a; scanf("%d", &a); a;})
#define LL ({ll  a; scanf("%lld", &a); a;})
#define FRD freopen("in.txt", "r", stdin)
#define FWR freopen("out.txt", "w", stdout)
using namespace std;
typedef unsigned long long ll;

#define rep(i,n) for( __typeof(n)i = 0 ; i < n ; i++)
#define For(i,n) for( __typeof(n)i = 1 ; i <= n ; i++)
#define forstl(i,n) for(__typeof(n.begin())i = n.begin();i!=n.end();i++)
#define forab(i,a,b)for(__typeof(b)i = a ; i <= (b) ; i++)

int main()
{
    //FRD;FWR;
    vector <int> v;
    vector <int> v1;
    int t,i,j,r,n,n1,m,k;
    cin>>t;
    cout<<"Case #"<<1<<":"<<" "<<"\n";
    cin>>r>>n>>m>>k;
    n1=n;
    int a[k];
    while(r)
    {
        for(j=0; j<k; j++)
        {
            cin>>a[j];
            v.push_back(a[j]);
        }
        sort(v.rbegin(),v.rend());
        for(i=m; i>=2; i--)
        {
            if(v[0]%i==0)
            {
                v[0]=v[0]/i;
                v1.push_back(i);
                if(v[0]%i==0)
                    v1.push_back(i);
                n--;
                if(n==0)
                    break;
            }
        }
        for(i=0; i<n1; i++)
            cout<<v1[i];
        v1.clear();
        v.clear();
        r--;
        cout<<endl;
    }
    return 0;
}
