#include <cmath>
#include <map>
#include <iostream>

using namespace std;

static int a[800];

void doit(long n) {a[0]=2; a[1]=6; int i; for(i=2;i<=n;i++) {a[i]=6*a[i-1]-4*a[i-2]; a[i]%=1000; a[i]+=1000; a[i]%=1000; if((a[i]==a[i/2]) && (a[i-1]==a[(i/2)-1
])) printf("%d\n", i-(i/2));}}

int main()
{
long N,m;
doit(400);
}
