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
char M[31][31];
int main()
{
    freopen("C-small-attempt1.in", "r", stdin);
    freopen("output.txt", "w", stdout);
    int t, Hd, Hk, Ad, Ak, B, D, ca=1, cur_h;
    cin>>t;
    while(ca<=t)
    {
        cout<<"Case #"<<ca<<": ";
        cin>>Hd>>Ad>>Hk>>Ak>>B>>D;
        cur_h=Hd;
        int c=0;
        bool b=1, b2=0;
        if(Ad>=Hk) c=1;
        else if(Ak>=Hd)b=0;
        else
        while(Hk>0)
        {
            //my move
            if(Ad>=Hk)
            {
                Hk-=Ad;
                c++;
                break;
            }
            if(Ak>=cur_h)
            {
                if(Ak-D<=0)
                {
                    Ak=0;
                    b2=0;
                }
                else
                {
                    int x=ceil(1.0*cur_h/(Ak-D));
                    int y=ceil(1.0*Hd/Ak);
                    if(x>=y)
                    {
                        Ak-=D;
                        b2=0;
                    }
                    else
                    {
                        cur_h=Hd;
                        if(b2){b=0;break;}
                        b2=1;
                    }
                }
            }
            else
            {
                b2=0;
                int x=ceil(1.0*Hk/Ad);
                int y=ceil(1.0*Hk/(Ad+B));
                if(x-y>1)
                    Ad+=B;
                else
                    Hk-=Ad;
            }
            //opponent's move
            cur_h-=Ak;
            c++;
        }
        if(b==0)
        {
            cout<<"IMPOSSIBLE\n";
        }
        else
            cout<<c<<endl;
        ca++;
    }
}
