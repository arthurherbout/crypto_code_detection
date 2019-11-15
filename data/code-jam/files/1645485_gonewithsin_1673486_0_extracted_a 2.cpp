#include<iostream>
#include<stdio.h>
#include<string.h>
#include<iomanip>
using namespace std;

const int maxint=0x7ffffff;
int t,a,b;
double p[100005];
double d[100005],e[100005];
double ans,ans1,ans2,ans3;

int main()
{
    //
    freopen("A-small-attempt2.in","r",stdin);
    //
    freopen("out.out","w",stdout);
    scanf("%d",&t);
    p[0]=1;
    for(int k=1;k<=t;k++)
    {
        scanf("%d%d",&a,&b);
        for(int i=1;i<=a;i++)
            cin>>p[i];
        d[0]=1;
        ans1=ans2=ans3=0;
        for(int i=1;i<=a;i++)
            d[i]=d[i-1]*p[i],e[i]=d[i-1]*(1-p[i]);
        e[a+1]=d[a];
        //for(int i=1;i<=a+1;i++)ans2+=e[i];cout<<ans2<<endl;
        for(int i=1;i<=a+1;i++)
        {
            if(i!=a+1)ans1+=e[i]*(2*b-a+2);
                else ans1+=e[a+1]*(b-a+1);
            //ans3+=e[i]*(2+b);
        }
        ans3=2+b;
        double z;
        ans2=maxint;
        for(int i=1;i<=a;i++)
        {
            z=d[a-i]*(i+(b-a+i)+1)+(1-d[a-i])*(i+(b-a+i)+1+b+1);
            ans2=min(ans2,z);
        }
        //?
        printf("Case #%d: ",k);
        //cout<<"!"<<ans1<<" "<<ans2<<" "<<ans3<<endl;
        ans=min(ans1,min(ans2,ans3));
        cout<<fixed<<setprecision(6)<<ans<<endl;
    }
    return 0;
}
