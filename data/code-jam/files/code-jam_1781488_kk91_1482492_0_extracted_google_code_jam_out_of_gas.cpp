#include<iostream>
#include<cmath>
#include<cstdio>
using namespace std;
int main()
{
   // freopen("inp.txt","r",stdin);
   // freopen("out.txt","w",stdout);
    int test,n,a,i,j,k;
    double d,time[3],dist[3],car1,car2,tempd,tempt,acc[12];
    cin>>test;
    for(i=0;i<test;i++)
    {
        cin>>d>>n>>a;
      //  cout<<"d:"<<d<<"n;"<<n<<"a:"<<a<<endl;
        car2=0;tempd=0;tempt=0;
        for(j=0;j<n;j++)
        {
           cin>>time[j]>>dist[j];
            if(dist[j]==d && car2==0)
            {
                car2=time[j];
            }
            else if(dist[j]>d && car2==0)
            {
                car2=(d-tempd)*((time[j]-tempt)/(dist[j]-tempd));
            }
            tempt=time[j];tempd=dist[j];
        }
        for(j=0;j<a;j++)
        cin>>acc[j];
        //  cout<<"car2:"<<car2<<endl;
        cout<<"Case #"<<i+1<<":"<<endl;
        for(j=0;j<a;j++)
        {
            car1=(sqrt((2*d)/acc[j]));
            if(car1>car2) printf("%.7lf\n",car1);
            else
            {
               printf("%.7lf\n",car2);
            }
        }
    }
    return 0;
}
