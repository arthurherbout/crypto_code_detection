#include <bits/stdc++.h>

#define mod 1000000007
#define inf 1000000000000LL
#define root2 1.41421
#define root3 1.73205
#define pi 3.14159
#define MAX 100001
#define cntbit __builtin_popcountll
#define ll long long int
#define PII pair<int, int>
#define f first
#define s second
#define mk make_pair
#define PLL pair<ll, ll>
#define gc getchar
#define pb push_back

using namespace std;
struct node{
    int x, p, q;
    string A, B;
};
bool compare(node a, node b)
{
    return a.x>b.x;
}
int main()
{
    freopen("B-small-attempt0.in", "r", stdin);
    freopen("output.txt", "w", stdout);
    int ca=1, t, n, r, o, y, g, b, v, x, z, i, y2;
    node X[3];
    cin>>t;
    string imp="IMPOSSIBLE";
    while(ca<=t)
    {
        cout<<"Case #"<<ca<<": ";ca++;
        cin>>n>>r>>o>>y2>>g>>b>>v;
        if(g>r||o>b||v>y2)
        {
            cout<<imp<<endl;
            continue;
        }
        x=r-g;
        y=b-o;
        z=y2-v;
        if(x>n/2||y>n/2||z>n/2)
        {
            cout<<imp<<endl;
            continue;
        }
        if((r==n/2&&x==0)||(b==n/2&&y==0)||(y2==n/2&&z==0))
        {
            for(i=0;i<r;i++)
                cout<<"RG";
            for(i=0;i<b;i++)
                cout<<"BO";
            for(i=0;i<y2;i++)
                cout<<"YV";
            cout<<endl;
            continue;
        }
        X[0].x=x;X[1].x=y;X[2].x=z;
        X[0].p=r;X[1].p=b;X[2].p=y2;
        X[0].q=g;X[1].q=o;X[2].q=v;
        X[0].A="RGR";X[1].A="BOB";X[2].A="YVY";
        X[0].B="R";X[1].B="B";X[2].B="Y";
        sort(X, X+3, compare);
        //solution possible
        //We have g RGR's and x-g R's
        x=X[0].x;
        y=X[1].x;
        z=X[2].x;
        r=X[0].p;
        b=X[1].p;
        y2=X[2].p;
        g=X[0].q;
        o=X[1].q;
        v=X[2].q;
        int k=y+z-x;
        while(r>0)
        {
            if(g>0)
            {
                cout<<X[0].A;
                g--;
                r-=2;
            }
            else if(r>0)
            {
                cout<<X[0].B;
                r--;
            }
            x--;
            if(o>0)
            {
                cout<<X[1].A;
                o--;
                b-=2;
                y--;
            }
            else if(y>0)
            {
                cout<<X[1].B;
                b--;
                y--;
            }
            else if(z>0)
            {
                if(v>0)
                {
                    cout<<X[2].A;
                    v--;
                    y2-=2;
                }
                else if(y2>0)
                {
                    cout<<X[2].B;
                    y2--;
                }
                z--;
            }
            if(k>0)
            {
                if(v>0)
                {
                    cout<<X[2].A;
                    v--;
                    y2-=2;
                }
                else if(y2>0)
                {
                    cout<<X[2].B;
                    y2--;
                }
                k--;
                z--;
            }
        }
        cout<<endl;
    }
    return 0;
}

