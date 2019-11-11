#include<cstring>
#include<iostream>
#include<cstdio>
#include<algorithm>
#include<vector>
using namespace std;

int A[55][55],Listid[55][55];

bool cmp(const pair<int,int> &a,const pair <int,int> &b)
{
    if(a.first==b.first)
        return (a.second<b.second);
    return (a.first<b.first);
}

bool cmp2(const vector<int> &a, const vector<int> &b)
{
    int n=a.size();
    for(int i=0;i<n;++i)
    {
        if(a[i]!=b[i])
            return (a[i]<b[i]);
    }
    return true;
}

int main()
{
//     freopen("ainput1_12.in","r",stdin);
//    freopen("cjout1.txt","w",stdout);

    int t,n,x;
    cin>>t;
    for(int tt=1;tt<=t;++tt)
    {
        cin>>n;
        vector <pair<int,int> > v;
        vector <int> Listid[2*n];
        for(int i=1;i<2*n;++i)
        {
            for(int j=0;j<n;++j)
            {
                cin>>x;
                Listid[i-1].push_back(x);
            }
            v.push_back(make_pair(Listid[i-1][0],i-1));
        }
        sort(v.begin(),v.end(),cmp);
        sort(Listid,Listid+2*n-1,cmp2);
        bool visit[55][55],mark[55];
        for(int i=0;i<55;++i)
        {
            mark[i]=false;
            for(int j=0;j<55;++j)
            visit[i][j]=false;
        }
        int row=0,col=0,id;
        bool conflict;
        for(int i=1;i<2*n;++i)
        {
            if(visit[row][0]==true)
            {
                mark[row]=true;
                id=v[i-1].second;
                conflict=false;
                for(int k=0;k<n;++k)
                {
                /*    if(visit[k][col]==true)
                    {
                        if(A[k][col]!=Listid[id][k])
                            conflict=true;
                    }
                    else
                  */
                    A[k][col]=Listid[id][k];
                    visit[k][col]=true;
                }
               /* if(conflict==true)
                {
                    int tmp;
                    for(int k=0;k<n;++k)
                    {
                        tmp=A[k][id];
                        A[k][id]=A[id][k];
                        A[id][k]=tmp;
                    }
                }*/
                ++col;
            }
            else
            {
                id=v[i-1].second;
                conflict=false;
                for(int k=0;k<n;++k)
                {
                   /* if(visit[row][k]==true)
                    {
                        if(A[row][k]!=Listid[id][k])
                            conflict=true;
                    }
                    else*/

                    A[row][k]=Listid[id][k];
                    visit[row][k]=true;
                }
                /*
                if(conflict==true)
                {
                    int tmp;
                    for(int k=0;k<n;++k)
                    {
                        tmp=A[k][id];
                        A[k][id]=A[id][k];
                        A[id][k]=tmp;
                    }
                }*/
                ++row;
            }
        }
        int missing;
        for(int i=0;i<n;++i)
        if(mark[i]==false)
        {
            missing=i;
            break;
        }
        bool match=true;
        for(int i=0;i<n;++i)
        {
            if(A[missing][i]!=Listid[2*missing][i])
            {
                match=false;
                break;
            }
        }
        cout<<"Case #"<<tt<<": ";
        if(match)
        {
            for(int i=0;i<n;++i)
                cout<<A[i][missing]<<" ";
            cout<<endl;
        }
        else
        {
            for(int i=0;i<n;++i)
                cout<<A[missing][i]<<" ";
            cout<<endl;
        }
        //cout<<"Case #"<<tt<<": "<<result<<endl;
    }
}

