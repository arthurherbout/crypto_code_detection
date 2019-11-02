#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <set>
#include <cmath>
#include <string.h>
#define TASK "C-small"
#define Max 10000
using namespace std;

int T,N;
long long L,H,F;
set<long long> Notes[Max];

void Init()
{
    for(int i=0; i<Max; i++)
    {
        Notes[i].clear();
    }
}

int main()
{
    freopen(TASK".in","r",stdin);
    freopen(TASK".out","w",stdout);
    
    cin>>T;
    
    for(int t=0; t<T; t++)
    {
        Init();
        cin>>N>>L>>H;
        
        for(int i=0; i<N; i++)
        {
            cin>>F;
            
            if(i==0)
            {
                //Get divisors and multiples
                if(F>=L && F<=H)
                {
                    Notes[i].insert(F);
                }
                long long s=sqrt(F);
                for(int d=1; d<=s; d++)
                {
                    if(F%d==0)
                    {
                        if(d>=L && d<=H)
                        {
                            Notes[i].insert(d);
                        }
                        long long dd=F/d;
                        if(dd>=L && dd<=H)
                        {
                            Notes[i].insert(dd);
                        }
                    }
                }
                
                long long mn=(L+F-1)/F,mx=H/F;
                
                for(long long m=mn; m<=mx; m++)
                {
                    Notes[i].insert(m*F);
                }
            }
            else
            {
                //Check for compatibility
                for(set<long long>::iterator it=Notes[i-1].begin(); it!=Notes[i-1].end(); it++)
                {
                    if((*it)%F==0 || F%(*it)==0)
                    {
                        Notes[i].insert((*it));
                    }
                }
            }
            
            /*cout<<"Printing "<<i<<endl;
            for(set<long long>::iterator it=Notes[i].begin(); it!=Notes[i].end(); it++)
            {
                cout<<(*it)<<" ";
            }
            cout<<endl;*/
            
        }
        
        
        //Output the lowest one
        cout<<"Case #"<<t+1<<": ";
        if(Notes[N-1].size()==0)
        {
            cout<<"NO"<<endl;
        }
        else
        {
            cout<<(*Notes[N-1].begin())<<endl;
        }
    }
    
    
    fclose(stdin);
    fclose(stdout);
    
    return 0;
}
