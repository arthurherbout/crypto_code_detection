#include<string.h>
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
struct pt {
	ll x, y;
	int in;
};

bool cmp (pt a, pt b) {
	return a.x < b.x || a.x == b.x && a.y < b.y;
}

bool cw (pt a,  pt b, pt c) {
	return a.x * (b.y-c.y) + b.x * (c.y-a.y) + c.x * (a.y-b.y) <=0;
}

bool ccw (pt a, pt b, pt c) {
	return a.x * (b.y-c.y) + b.x * (c.y-a.y) + c.x * (a.y-b.y)>= 0;
}

void convex_hull (vector<pt> & a) {
	if (a.size() == 1)  return;
	sort (a.begin(), a.end(), &cmp);
	pt p1 = a[0],  p2 = a.back();
	vector<pt> up, down;
	up.push_back (p1);
	down.push_back (p1);
	for (size_t i=1; i<a.size(); ++i) {
		if (i==a.size()-1 || cw (p1, a[i], p2)) {
			while (up.size()>=2 && !cw (up[up.size()-2], up[up.size()-1], a[i]))
				up.pop_back();
			up.push_back (a[i]);
		}
		if (i==a.size()-1 || ccw (p1, a[i], p2)) {
			while (down.size()>=2 && !ccw (down[down.size()-2], down[down.size()-1], a[i]))
				down.pop_back();
			down.push_back (a[i]);
		}
	}
	a.clear();
	for (size_t i=0; i<up.size(); ++i)
		a.push_back (up[i]);
	for (size_t i=down.size()-2; i>0; --i)
		a.push_back (down[i]);
}
int main()
{

int n,m;
int t;
scanf("%d",&t);
int ca;
for(ca=1;ca<=t;ca++)
{
printf("Case #%d:\n",ca);
    scanf("%d",&n);

    vector<pt> b;
    int i;
    for(i=0;i<n;i++)
    {
        pt cur;
        scanf("%lld%lld",&cur.x,&cur.y);
        cur.in =i;
        b.push_back(cur);
    
    }
   // convex_hull(b);
    int ma = 1<<n;
    int cc;
    for(i=0;i<n;i++){
        int mi = n+1;
    for(cc = 0;cc < ma;cc++)
    {
       // printf("we are selecting\n");
        vector<pt> a,c;
        int j;
        int rem = 0;
        for(j=0;j<n;j++)if( cc & (1<<j)){
            a.push_back(b[j]);
            c.push_back(b[j]);
       // printf("%d %lld %lld\n",j,b[j].x,b[j].y);
            
        }
        
            else rem ++;
        
        int found =0;
        if(a.size() > 3){
        convex_hull(a);
      //  printf("found a convex_hull\n");
      //  printf("we selected\n");
      //  for(j=0;j<c.size();j++)
      //  printf("%d %lld %lld\n",c[j].in,c[j].x,c[j].y);
        
     //   printf("hull is after removing %d \n",rem);
        //   for(j=0;j<a.size();j++)
      //  printf("%d %lld %lld\n",a[j].in,a[j].x,a[j].y);
        for(j=0;j<a.size();j++)
        {
            if(a[j].in ==i){found = 1;
           // printf("%d is in the hull after removing %d for %d\n",i,rem,cc);
            }
        }
        
        if(found){
//rem = n-c.size();
           if(rem < mi)mi = rem;
        }
        }
    }
if(n <=3)mi = 0;
    printf("%d\n",mi);
    
    }
 
}




return 0;
}
