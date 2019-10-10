#include <algorithm>
#include <iostream>
#include <vector>
#include <map>
#include <cmath>

using namespace std;

typedef long long ll;
typedef unsigned long long ull;
typedef unsigned long ul;
typedef unsigned short us;
typedef unsigned char uc;

ull isqrt(ull n)
{ull q=0,r=1LL<<62;while(q/=2,r/=4) n>=r+q?n-=r|q,q+=r<<1:0; return(q);}

main()
{
  int cases;
  cin >> cases;
  for(int loop=1; loop<=cases; loop++)
  {
    char s[80];
    printf("Case #%d: ",loop);
    scanf("%s",s);
    int n=strlen(s); int k=0; int pos[20];
    for(int i=0;i<n;i++) if(s[i]=='?') { pos[k++]=n-1-i; s[i]='0'; }
    ull base=0;

    for(int i=0;i<n;i++) { base*=2; base+=(s[i]-'0'); }
//fprintf(stderr,"starting at %llu\n",base);

    for(int i=0;i<(1<<k);i++)
    {
      ull t=base;
      for(int j=0;j<k;j++) if(i&(1<<j)) t+=(1LL<<pos[j]);
      ull q=isqrt(t); if (q*q==t)
      {
        for(int i=n-1;i>=0;i--) { s[i]='0'+(t%2); t/=2; }
        puts(s); break;
      }
    }
  }
}
