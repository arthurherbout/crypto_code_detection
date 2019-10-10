#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <queue>
#include <deque>
#include <set>

using namespace std;

#define mem(i,n) memset(i,n,sizeof(i))
#define fo(i,n) for(i=0;i<(n);i++)
#define all(x) x.begin(),x.end()
#define pb push_back
#define sz size()
#define cs c_str()

typedef vector<int> vi;
typedef vector<string> vs;
typedef vector<double> vd;

FILE *in=fopen("B-small-attempt0.in","r");
FILE *out=fopen("B.out","w");

string s;

long long solve(int ind,long long total,string last,int lsign)
{
    if(ind==s.sz)
    {
        long long x,y;
        sscanf(last.cs,"%lld",&x);
        if(lsign==1)y=total+x;
        else y=total-x;
        if(abs(y)%2==0 || abs(y)%3==0 || abs(y)%5==0 || abs(y)%7==0)return 1;
        else return 0;
    }
    long long ret=0;
    string h;
    h=last;
    h+=s[ind];
    ret+=solve(ind+1,total,h,lsign);
    long long x;
    sscanf(last.cs,"%lld",&x);
    if(lsign==2)x=-x;
    h=s[ind];
    ret+=solve(ind+1,total+x,h,1);
    ret+=solve(ind+1,total+x,h,2);
    return ret;
}

int main()
{
    long long i,p,j,k,N;
    fscanf(in,"%lld",&N);
    fo(p,N)
    {printf("done\n");
        char x[50];
        fscanf(in,"%s",x);
        s=x;
        string h;
        h+=s[0];
        fprintf(out,"Case #%lld: %lld\n",p+1,solve(1,0,h,1));
    }
    return 0;
}

































