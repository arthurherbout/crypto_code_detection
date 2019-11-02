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
    freopen("A-large.in", "r", stdin);
    freopen("output.txt", "w", stdout);
    int t, r, c, ca=1, i, j;
    cin>>t;
    while(ca<=t)
    {
        cout<<"Case #"<<ca<<":\n";
        cin>>r>>c;
        for(i=0;i<r;i++)
        {
            for(j=0;j<c;j++)
            {
                char ch=gc();
                while(ch==' '||ch=='\n')ch=gc();
                M[i][j]=ch;
            }
        }
        char lc='-';
        bool r1=0;
        bool z=0;
        for(i=0;i<r;i++)
        {
            lc='-';
            for(j=0;j<c;j++)
            {
                if(M[i][j]=='?'&&lc!='-')
                    M[i][j]=lc;
                else if(M[i][j]!='?'&&lc=='-')
                {
                    for(int k=0;k<j;k++)
                        M[i][k]=M[i][j];
                    if(r1==0)z=1;
                }
                if(M[i][j]!='?')
                    lc=M[i][j];
            }
            if(lc=='-'&&r1)
            {
                for(j=0;j<c;j++)
                M[i][j]=M[i-1][j];
            }
            if(z)
            {
                z=0;r1=1;
                for(int k=0;k<i;k++)
                for(j=0;j<c;j++)
                M[k][j]=M[i][j];
            }
        }
        for(i=0;i<r;i++)
        {
            for(j=0;j<c;j++)
            {
                cout<<M[i][j];
            }
            cout<<endl;
        }
        ca++;
    }
}
