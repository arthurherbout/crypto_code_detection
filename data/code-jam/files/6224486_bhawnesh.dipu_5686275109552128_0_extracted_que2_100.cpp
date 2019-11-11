#include<iostream>
#include<vector>
#include<numeric>
#include<fstream>
#include<algorithm>
using namespace std;

void _divide(vector <int> &v,int &n,int i)
{
    if(v[i]%2==0)
    {
        v.push_back(v[i]/2);
        v[i]=v[i]/2;
        n=n+1;
    }
    else if(v[i]%2==1)
    {
        v.push_back(v[i]/2+1);
        v[i]=v[i]/2;
        n=n+1;

    }

}


        int v_min[10]={0,1,2,3,3,4,4,5,5,6};
        int v_22[10]={0,0,0,1,1,2,3,4,5,6};
        int v_21[10]={0,0,1,2,3,4,5,6,7,8};
        int v_20[10]={0,1,2,3,4,5,6,7,8,9};


        int v_24[10]={0,0,0,0,0,1,1,1,1,2};
        int v_23[10]={0,0,0,0,1,1,1,2,2,2};
        int v_25[10]={0,0,0,0,0,0,1,1,1,1};
        int v_26[10]={0,0,0,0,0,0,0,1,1,1};
        int v_27[10]={0,0,0,0,0,0,0,0,1,1};
        int v_28[10]={0,0,0,0,0,0,0,0,0,1};
        int v_29[10]={0,0,0,0,0,0,0,0,0,0};
int main()
{

      freopen("B-small-attempt5.in","r",stdin);
    freopen("B-small-attempt5.out","w",stdout);
    int tt;
    cin>>tt;
    for(int test=1;test<=tt;test++)
    {
        int d;
        cin>>d;
        vector <int> arr(d,0);
        int answer=0;
        for(int i=0;i<d;i++)
        {
            cin>>arr[i];
        //cout<<arr[i]<<"  ";
        }
        sort(arr.begin(),arr.end());
        int max1=arr[d-1];
        vector <int> ans(10,0);
        if(d==1)
        {
            answer=v_min[arr[0]];

        }
        else
        {


        for(int i=0;i<d;i++)
        {
            ans[0]+=v_20[arr[i]];
            ans[1]+=v_21[arr[i]];
            ans[2]+=v_22[arr[i]];
                    ans[3]+=v_23[arr[i]];
                    ans[4]+=v_24[arr[i]];
                    ans[5]+=v_25[arr[i]];

                    ans[6]+=v_26[arr[i]];
                    ans[7]+=v_28[arr[i]];
                    ans[8]+=v_28[arr[i]];

                    ans[9]+=v_29[arr[i]];

        }
        for(int j=0;j<10;j++)
        {
            ans[j]+=j;

        }
        sort(ans.begin(),ans.end());
         answer=ans[0];
        }
        //cout<<"max1="<<max1<<"ans=="<<ans<<"  ";
        //if(ans>max1)ans=max1;

        cout<<"Case #"<<test<<": "<<answer<<endl;

    }

}
