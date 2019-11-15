#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <ctime>
#include <vector>
#include <set>
#include <map>
#include <queue>
#include <deque>
#include <string.h>
#include <stack>
#include <sstream>
#include <bitset>
#include <algorithm>
#include <numeric>
#define TASK "B-small"
#define Max 1000000
using namespace std;

long long T,L,t,N,C, Star[Max],Dist[Max+1];
priority_queue<long long> pq;

void Init()
{
    memset(Star,0,sizeof(Star));
    memset(Dist,0,sizeof(Dist));
    while(!pq.empty())pq.pop();
}

int main()
{
    freopen(TASK".in","r",stdin);
    freopen(TASK".out","w",stdout);
    
    cin>>T;
    
    for(int test=0; test<T; test++)
    {
        Init();
        cin>>L>>t>>N>>C;
        
        for(int i=0; i<C; i++)
        {
            cin>>Star[i];
            Dist[i+1]=Dist[i]+Star[i];
            
            if(t>Dist[i])//If the segment won't be complete
            {
                if(t/2<Dist[i+1])//If the segment won't be overlapped
                {
                    pq.push(Dist[i+1]-t/2);
                }
            }
            else
            {
                pq.push(Star[i]);
            }
        }
        
        for(int i=C; i<N; i++)
        {
            Star[i]=Star[i%C];
            pq.push(Star[i]);
            Dist[i+1]=Dist[i]+Star[i];
        }
        
        long long Time=2LL*Dist[N];
        
        for(int i=0; i<L && !pq.empty(); i++)
        {
            Time-=pq.top();
            pq.pop();
        }
        
        cout<<"Case #"<<test+1<<": "<<Time<<endl;
    }
    
    
    fclose(stdin);
    fclose(stdout);
    
    return 0;
}
