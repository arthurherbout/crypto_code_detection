#include <algorithm>
#include <iostream>
#include <vector>
#include <map>
#include <set>

using namespace std;

typedef long long ll;
typedef unsigned long long ull;
typedef unsigned long ul;
typedef unsigned short us;
typedef unsigned char uc;

int gcd(int x,int y){int t;if(!x)return y;for(y%=x;y;t=y,y=x%y,x=t);return x;}

main()
{
  int cases;
  cin >> cases;
  for(int loop=1; loop<=cases; loop++)
  {
    int h,w,d;
    printf("Case #%d: ",loop);
    cin >> h >> w >> d;
    set<pair<int, int> > S;
    char M[50][50];
    for(int i=0;i<h;i++) scanf("%s",M[i]);

    int r, c;
    for(int i=0;i<h;i++) for(int j=0;j<w;j++) if (M[i][j]=='X') {r=i;c=j;}

    r--; c--; h-=2; w-=2;
    //printf("(%d,%d) in %dx%d\n",r,c,h,w);
    int R = 55;
    for(int x=-R;x<=R;x++) for(int y=-R;y<=R;y++)
    {
      int dc,dr;
      if (x==0&&y==0) continue;
      if (x%2==0) { dc = w*x; } else { dc = w*x + 2*c + w+1; }
      if (y%2==0) { dr = h*y; } else { dr = h*y + 2*r + h+1; }
      int d2 = dr*dr+dc*dc;
      if (d2>0 && d2<=d*d)
      {
        //printf("Got <%d,%d>\n",dr,dc);
        int g=gcd(abs(dr),abs(dc)); dr /= g; dc /= g;
        S.insert(make_pair(dr,dc));
        //cout << S.size() << endl;
      }
    }

    cout << S.size() << endl;
  }
}
