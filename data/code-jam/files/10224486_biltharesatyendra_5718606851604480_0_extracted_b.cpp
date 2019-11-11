#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
  char a[55];
char b[55];
char c[55];
  int n,p,r,s,m;
int solve()
    {
    int total = m;
    int in ,i;
    for(i=0;i<total;i++)b[i] = a[i];
    while(1)
        {
        if(total==1)return 1;
        in = 0;
        for(i=0;i<total;i+=2)
            {
            if(b[i]==b[i+1])return 0;
            /*
             Rock beats Scissors, Scissors beats Paper, and Paper beats Rock. I
             */
            if(b[i]=='S' && b[i+1]=='R')c[in] = 'R';
            if(b[i]=='R' && b[i+1]=='S')c[in] = 'R';
            if(b[i]=='S' && b[i+1]=='P')c[in] = 'S';
            if(b[i]=='P' && b[i+1]=='S')c[in] = 'S';
            if(b[i]=='R' && b[i+1]=='P')c[in] = 'P';
            if(b[i]=='P' && b[i+1]=='R')c[in] = 'P';
            in++;
        }
        total = in;
    }
}
int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */   
    int t,ca;
    scanf("%d",&t);
    for(ca=1;ca<=t;ca++)
        {
     
        scanf("%d%d%d%d",&n,&r,&p,&s);
      int in = 0;
        int i;
        for(i=0;i<r;i++)a[in++] = 'R';
        for(i=0;i<p;i++)a[in++] = 'P';
        for(i=0;i<s;i++)a[in++] = 'S';
        a[in] = 0;
        int valid = 0;
        m = 1<<n;
        sort(a,a+m);
        while(1)
            {
            //printf("%s\n",a);
             
            if(solve()){
                valid = 1;break;
            }
            
            if(!next_permutation(a,a+m))break;
        }
        if(valid)printf("Case #%d: %s\n",ca,a);
        else printf("Case #%d: IMPOSSIBLE\n",ca);
    }
    return 0;
}

