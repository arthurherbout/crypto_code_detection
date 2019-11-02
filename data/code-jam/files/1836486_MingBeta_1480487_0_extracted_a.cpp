#include<iostream>

using namespace std;

struct node{
       int id;
       double s;
       } a[300];

bool cmp1(const node &x,const node &y)
{
     if (x.s>y.s)return true;
     return false;
}

int main()
{
    freopen("A-small-attempt3.in","r",stdin);
    freopen("A-small-attempt3.out","w",stdout);
    int test;
    cin>>test;
    int n;
    double tot,ans[205],tmp;
    for (int t=1;t<=test;t++){
        cin>>n;
        tot=0;
        int flag=n;
        for (int i=0;i<n;i++){
            cin>>a[i].s;
            a[i].id=i;
            tot+=a[i].s*1.0;
            }
        sort(a,a+n,cmp1);
        tmp=0;
        memset(ans,0,sizeof(ans));
        //for (int i=0;i<n;i++) cout<<a[i].s<<" "<<a[i].id<<endl;
        for (int i=0;i<n;i++){
            if (a[i].s>(tot*2-tmp)/(flag*1.0)){
               ans[a[i].id]=0.0;
               tmp+=a[i].s;
               flag--;
               }
               else 
               {
               ans[a[i].id]=100.0*((tot*2-tmp)/(flag*1.0)-a[i].s)/tot;
               }
               //cout<<a[i].id<<" "<<ans[a[i].id]<<endl;
            }
        cout<<"Case #"<<t<<": ";
        for (int i=0;i<n;i++)
            printf("%.5lf ",ans[i]);
        printf("\n");
        }
}
