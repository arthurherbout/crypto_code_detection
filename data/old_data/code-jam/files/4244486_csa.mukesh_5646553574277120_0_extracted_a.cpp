#include <iostream>
#include <cstdio>
#include <algorithm>
#include <string>
#include <cstring>
#include <cmath>
#include <queue>
#include <map>
#include <vector>
#include <stack>
#include <set>
#include <fstream>
#include <iomanip>
#define mp make_pair
#define M 10000
#define limit 150000
#define Inf 1e9+1
#define accr 1e-6
#define PI 3.141592653589793238462643383279502884197
#define ll (long long)
#define ld long double
#define ff first.first
#define fs first.second
#define Mod 1000000007
#define sec second
using namespace std;
int main()
{
    int i,j,k,t,Case;
    freopen("file.in","r",stdin);
    freopen("file.out","w+",stdout);
 	cin>>t;
    Case = 0;
    while(t--)
    {
        Case++;
        set<int> s,s1,s2,dens,alrd,req,node[31];
        set<int> ::iterator it,it1,it2,it3,it4,it5;
        bool found;
        int c,d,v,ans;
        
        cin>>c>>d>>v;
        int den[d];
        for(i=1;i<=v;i++) s1.insert(i);
        for(i=0;i<d;i++)
        {
            cin>>den[i];
            dens.insert(den[i]);
            s1.erase(den[i]);
        }
        s.insert(0);
        for(i=0;i<d;i++)
        {
        for(it=s.begin();it!=s.end();it++)
        {
            if(*it+den[i]<=v)
            {
                s2.insert(*it+den[i]);
            }
        }
        for(it=s2.begin();it!=s2.end();it++)
        {
            s.insert(*it);
        }
        s2.clear();
        }
       // cout<<s.size()<<endl;
        found=false;

        if(s.size()==v+1)
        {
            found=true;
            ans =0;
        }

        s2=s;
        if(!found)
        {
            for(it1=s1.begin();it1!=s1.end()&&!found;it1++)
            {
            	// cout<<*it1<<" "<<*it2<<" "<<*it3<<endl;
                s2 = s;
                req = s2;
                for(it=s2.begin();it!=s2.end();it++)
                {
                	// cout<<*it1<<" "<<*it2<<" "<<*it3<<endl;
                    if(*it1+*it<=v)
                    {
                        req.insert(*it1+*it);
                    }
                }
                // cout<<*it1<<" "<<*it2<<" "<<*it3<<endl;
                if(req.size()==v+1)
                {
                    found=true;
                    ans=1;
                }
            }
        }
        s2=s;
        if(!found)
        {
            for(it1=s1.begin();it1!=s1.end()&&!found;it1++)
            {
            // cout<<*it1<<" "<<*it2<<" "<<*it3<<endl;
            for(it2=s1.begin();it2!=it1&&!found;it2++)
            {
            // cout<<*it1<<" "<<*it2<<" "<<*it3<<endl;
                s2 = s;
                req = s2;
                for(it=s2.begin();it!=s2.end();it++)
                {
                // cout<<*it1<<" "<<*it2<<" "<<*it3<<endl;
                    if(*it1+*it<=v)
                    {
                        req.insert(*it1+*it);
                    }
                }
                // cout<<*it1<<" "<<*it2<<" "<<*it3<<endl;
                s2=req;
                for(it=s2.begin();it!=s2.end();it++)
                {
                // cout<<*it1<<" "<<*it2<<" "<<*it3<<endl;
                    if(*it2+*it<=v)
                    {
                        req.insert(*it2+*it);
                    }
                }
                
                // cout<<*it1<<" "<<*it2<<" "<<*it3<<endl;
                if(req.size()==v+1)
                {
                    found=true;
                    ans=2;
                }
            }
            }
        }

        if(!found)
        {
            for(it1=s1.begin();it1!=s1.end()&&!found;it1++)
            {
            for(it2=s1.begin();it2!=it1&&!found;it2++)
            {
            for(it3=s1.begin();it3!=it2&&!found;it3++)
            {
                s2 = s;
                req = s2;
                for(it=s2.begin();it!=s2.end();it++)
                {
                    if(*it1+*it<=v)
                    {
                        req.insert(*it1+*it);
                    }
                }
                s2=req;
                for(it=s2.begin();it!=s2.end();it++)
                {
                    if(*it2+*it<=v)
                    {
                        req.insert(*it2+*it);
                    }
                }
                s2=req;
                for(it=s2.begin();it!=s2.end();it++)
                {
                    if(*it3+*it<=v)
                    {
                        req.insert(*it3+*it);
                    }
                }
                if(req.size()==v+1)
                {
                    found=true;
                    ans=3;
                }
            }
            }
            }
        }
		// cout<<*it1<<" "<<*it2<<" "<<*it3<<endl;
        if(!found)
        {
            for(it1=s1.begin();it1!=s1.end()&&!found;it1++)
            {
            // cout<<*it1<<" "<<*it2<<" "<<*it3<<endl;
            for(it2=s1.begin();it2!=it1&&!found;it2++)
            {
            // cout<<*it1<<" "<<*it2<<" "<<*it3<<endl;
            for(it3=s1.begin();it3!=it2&&!found;it3++)
            {
            // cout<<*it1<<" "<<*it2<<" "<<*it3<<endl;
            for(it4=s1.begin();it4!=it3&&!found;it4++)
            {
            	// cout<<*it1<<" "<<*it2<<" "<<*it3<<endl;
                s2 = s;
                req = s2;
                for(it=s2.begin();it!=s2.end();it++)
                {
                    if(*it1+*it<=v)
                    {
                        req.insert(*it1+*it);
                    }
                }
                // cout<<*it1<<" "<<*it2<<" "<<*it3<<endl;
                s2=req;
                for(it=s2.begin();it!=s2.end();it++)
                {
                    if(*it2+*it<=v)
                    {
                        req.insert(*it2+*it);
                    }
                }
                s2=req;
                for(it=s2.begin();it!=s2.end();it++)
                {
                    if(*it3+*it<=v)
                    {
                        req.insert(*it3+*it);
                    }
                }
                s2=req;
                // cout<<*it1<<" "<<*it2<<" "<<*it3<<endl;
                for(it=s2.begin();it!=s2.end();it++)
                {
                    if(*it4+*it<=v)
                    {
                        req.insert(*it4+*it);
                    }
                }
                if(req.size()==v+1)
                {
                    found=true;
                   // cout<<*it1<<" "<<*it2<<" "<<*it3<<endl;
                    ans=4;
                }
            }
            }
            }
            }
        }


        if(!found)
        {
            for(it1=s1.begin();it1!=s1.end()&&!found;it1++)
            {
            // cout<<*it1<<" "<<*it2<<" "<<*it3<<endl;
            for(it2=s1.begin();it2!=it1&&!found;it2++)
            {
            // cout<<*it1<<" "<<*it2<<" "<<*it3<<endl;
            for(it3=s1.begin();it3!=it2&&!found;it3++)
            // cout<<*it1<<" "<<*it2<<" "<<*it3<<endl;
            {
            for(it4=s1.begin();it4!=it3&&!found;it4++)
            {
            // cout<<*it1<<" "<<*it2<<" "<<*it3<<endl;
            for(it5=s1.begin();it5!=it4&&!found;it5++)
            {
            // cout<<*it1<<" "<<*it2<<" "<<*it3<<endl;
                s2 = s;
                req = s2;
                for(it=s2.begin();it!=s2.end();it++)
                {
                    if(*it1+*it<=v)
                    {
                        req.insert(*it1+*it);
                    }
                }
                // cout<<*it1<<" "<<*it2<<" "<<*it3<<endl;
                s2=req;
                for(it=s2.begin();it!=s2.end();it++)
                {
                    if(*it2+*it<=v)
                    {
                        req.insert(*it2+*it);
                    }
                }
                // cout<<*it1<<" "<<*it2<<" "<<*it3<<endl;
                s2=req;
                for(it=s2.begin();it!=s2.end();it++)
                {
                    if(*it3+*it<=v)
                    {
                        req.insert(*it3+*it);
                    }
                }
                // cout<<*it1<<" "<<*it2<<" "<<*it3<<endl;
                s2=req;
                for(it=s2.begin();it!=s2.end();it++)
                {
                    if(*it4+*it<=v)
                    {
                        req.insert(*it4+*it);
                    }
                }
                // cout<<*it1<<" "<<*it2<<" "<<*it3<<endl;
                s2=req;
                for(it=s2.begin();it!=s2.end();it++)
                {
                    if(*it5+*it<=v)
                    {
                        req.insert(*it5+*it);
                    }
                }
                if(req.size()==v+1)
                {
                    found=true;
                   // cout<<*it1<<" "<<*it2<<" "<<*it3<<endl;
                    ans=5;
                }
            }
            // cout<<*it1<<" "<<*it2<<" "<<*it3<<endl;
            }
            // cout<<*it1<<" "<<*it2<<" "<<*it3<<endl;
            }
            // cout<<*it1<<" "<<*it2<<" "<<*it3<<endl;
            }
            // cout<<*it1<<" "<<*it2<<" "<<*it3<<endl;
            }
        }



        cout<<"Case #"<<Case<<": "<<ans<<endl;
        }
    return 0;
 }
