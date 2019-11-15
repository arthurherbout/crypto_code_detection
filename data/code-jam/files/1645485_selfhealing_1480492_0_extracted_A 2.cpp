#include <cstdio>
#include <cstring>
#include <vector>
#include <cmath>
#include <algorithm>
using namespace std;

struct Node
{
	int l,v;
	double x;
    Node() { }
    Node(const int& l, const int& v, const double& x) :
        l(l), v(v), x(x) { }
};

double ans;
int inf, n;
char s[20];
const double eps = 1e-8;
const int Len = 5;
const double Inf = 1e20;

void sol(vector<Node> a, double st, int id)
{
//	printf("%f:\n",st);
//	for(int i = 0; i < n; i++)
//		printf("%d %d %f\n",a[i].l,a[i].v,a[i].x);
//	puts("");

	if(inf)return;
	int ok = 0;
	double mx = Inf;
	int id1,id2;
	for(int i = 0; i < n; i++)
		for(int j = i+1; j < n; j++)
			if(a[i].l==a[j].l)
			{
				if(fabs(a[i].x-a[j].x)+eps<5)return;
				if(a[i].x < a[j].x)
				{
					if(a[i].v <= a[j].v)continue;
					else
					{
						ok = 1;
						double t = (fabs(a[j].x-a[i].x)-Len)/(a[i].v-a[j].v);
						if(mx > t)
						{
							mx = t;
							id1=i, id2=j;
						}
					}
				}
				else
				{
					if(a[i].v >= a[j].v)continue;
					else
					{
						ok = 1;
						double t = (fabs(a[j].x-a[i].x)-Len)/(a[j].v-a[i].v);
						if(mx > t)
						{
							mx = t;
							id1=i, id2=j;
						}
					}
				}
			}
//	puts("==");
	if(!ok)
	{
		inf = 1;
		return;
	}
	if(fabs(mx)<eps && (id==id1||id==id2))return;
	ans = max(ans,mx+st);
	vector<Node> b,bb;b.clear();
	for(int i = 0; i < n; i++)
		if(i!=id1 && i!=id2)
		{
			b.push_back(Node(a[i].l,a[i].v,a[i].x+a[i].v*mx));
		}
	bb = b;
	b.push_back(Node(a[id1].l,a[id1].v,a[id1].x+a[id1].v*mx));
	b.push_back(Node(a[id2].l^1,a[id2].v,a[id2].x+a[id2].v*mx));
	sol(b,st+mx,id2);
	bb.push_back(Node(a[id2].l,a[id2].v,a[id2].x+a[id2].v*mx));
	bb.push_back(Node(a[id1].l^1,a[id1].v,a[id1].x+a[id1].v*mx));
	sol(bb,st+mx,id1);
}

int main()
{
	freopen("in.txt","r",stdin);
	freopen("out.txt","w",stdout);
	int T,cas=0;scanf("%d",&T);
	while(T--)
	{
		scanf("%d",&n);
		vector<Node> a;
		for(int i = 0; i < n; i++)
		{
			int v,p;
			scanf("%s%d%d",s,&v,&p);
			a.push_back(Node(s[0]=='R',v,p*1.0));
		}
		ans = 0;inf = 0;
		sol(a,0,-1);
		printf("Case #%d: ",++cas);
		if(inf)printf("Possible\n");
		else printf("%.5f\n",ans);
	}

	return 0;
}
