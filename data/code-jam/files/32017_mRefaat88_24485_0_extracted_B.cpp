#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <cmath>
#include <algorithm>
#include <functional>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include<stack>
#include<memory.h>
#include<queue>
#include<map>
#include<set>
using namespace std;

const int SIZE = 1001;
struct DisjointSet 
{
       int parent[SIZE], rank[SIZE];
       int comp; // number of components
       DisjointSet(int c)
       {
                    comp = c;
                    memset(rank,0,sizeof(rank)); 
                    for(int i = 0 ; i < c ; i ++)
                    {
                            parent[i] = i; // all nodes are parents of thier selves
                    }
       }
       
       int find(int x)
       {
           return parent[x] == x ? x : parent[x] = find(parent[x]); // find node parent -- exist on any component 
       }

       // merge 2 components with node x and node y
       void join(int x, int y)
       {
           int xf = find(x);
           int yf = find(y);
           if(xf == yf)return;
           if(rank[xf] ==  rank[yf])rank[xf]++;
           if(rank[xf] < rank[yf])swap(xf,yf);
           parent[xf] = yf;
           comp --;
       }
};

vector< pair<int,int> > factorize (int num) 
{
	int i,d = 1;
	int n = num;
	vector< pair<int,int> >   result;
	for( i = 2 ; i*i <= n ; i +=d, d = 2)
	{
				if(n%i == 0)
				result.push_back(make_pair(i,0));
				while(n%i == 0)
				{
					n /= i;
					result.back().second ++;
				}
	}
	if(n!=1)
	result.push_back(make_pair(n,1));
	
	return result;
} 

bool shareFactor(int P,int a,int b)
{
    vector< pair<int,int> > t1 = factorize(a);
    vector< pair<int,int> > t2 = factorize(b);
    set<int> all;
    int c = 0;
    for(int i = 0 ; i < t1.size() ; i ++)
    {
        if(t1[i].first>=P)all.insert(t1[i].first),c++;
    }
    for(int i = 0 ; i < t2.size() ; i ++)
    {
        if(t2[i].first>=P)all.insert(t2[i].first),c++;
    }
    return c>all.size();
}
int main()
{
     freopen("B-small-attempt2.in","rt",stdin);
  freopen("B-small-attempt2.out","wt",stdout);
 int C,A,B,P;
 cin>>C;

 
 
 for(int l = 0 ; l < C ; l ++)
 {
        cin>>A>>B>>P;
        DisjointSet tt(B-A+1);
        int st = A;
        
        for(int i = A ; i <= B ; i++)
        {
            for(int j = i+1 ; j <= B ; j ++)
            {
                if(shareFactor(P,i,j))
                {
                    tt.join(i-A,j-A);
                }
            }
        }
        cout<<"Case #"<<l+1<<": "<<tt.comp<<endl; 
 }
 
  return 0;      
  
}
