#include <cstdlib>
#include <iostream>
#include <cmath>
#include <iomanip>
#define N 2010
#define M 300

using namespace std;

double x[N],y[N],z[M];

int main(int argc, char *argv[])
{
    int i,j,k;
    int u,o;
    double h;
    int n,m;
    int r;
    double v,s,t,g,ns,os,st;
    freopen("B-small-attempt0.in", "r", stdin);
    freopen("b.out", "w", stdout);
    cin>>u;
    o=1;
    while(u--)
    {
        cin>>h;
        cin>>n>>m;
        for(i=0,r=n;i<n;i++)
        {
            cin>>y[i]>>x[i];
            if(x[i]==h&&r==n)
            {
                r=i+1;
            }
            if(r==n&&x[i]>h)
            {
                r=i+1;
                v=(x[i]-x[i-1])/(y[i]-y[i-1]);
                s=x[i]-h;
                t=s/v;
                x[i]=h;
                y[i]=t+y[i-1];
            }
        }
        n=r;
        if(x[n-1]!=h)
        {
            x[n+1]=h;
            v=(x[n]-x[n-1])/(y[n]-y[n-1]);
            y[n+1]=y[n]+(h-x[n])/v;
            n++;
        }
        for(i=0;i<m;i++)
        {
            cin>>z[i];
        }
        cout<<"Case #"<<o<<":"<<endl;
        for(i=0;i<m;i++)
        {
            for(j=1,v=0,s=0;j<n;j++)
            {
                os=x[j]-x[j-1];
                t=y[j]-y[j-1];
                ns=x[j]-s;
                g=(2.0*(ns-v*t))/(t*t);
              //  cout<<"^"<<ns<<" "<<g<<" "<<z[i]<<endl;
                if(g>z[i])
                {
                    //v=v+z[i]*t;
                    s=s+v*t+0.5*z[i]*t*t;
                    v=v+z[i]*t;
                 //   cout<<"^^"<<s<<" "<<v<<" "<<x[j]<<endl;
                }
                else
                {
                    s=x[j];
                    if(g<0)
                        v=os/t;
                    else
                        v=v+g*t;
                }
            }
            if(s==h)
                cout<<fixed<<setprecision(7)<<y[n-1]<<endl;
            else
            {
                ns=h-s;
                t=(-2.0*v+2.0*sqrt(v*v+2.0*ns*z[i]))/2.0*z[i];
                t=t+y[n-1];
                cout<<fixed<<setprecision(7)<<t<<endl;
            }
        }
        o++;
    }
   // system("PAUSE");
    return EXIT_SUCCESS;
}
