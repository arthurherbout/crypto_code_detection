#include <stdio.h>
#include <algorithm>
#include <vector>
using namespace std;

struct point{
	point(double x_, double y_){
		x = x_; y = y_;
	}
	double x,y;
	bool operator <(const point t) const{
		if (x == t.x) return y > t.y;
		return x < t.x;
	}
};

double ccw(point a, point b, point c)
{
	return a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y);
}

vector<point> push(vector<point> p, point u)
{
	while (p.size() >= 2){
		int z = p.size();
		if (ccw(p[z-2],p[z-1],u) >= 0) p.pop_back();
		else break;
	}
	p.push_back(u);
	return p;
}

double time(vector<point> p, double x, double y)
{
	while (p.size() >= 2){
		int z = p.size();
		if (ccw(p[z-2],p[z-1],point(x,y)) >= 0) p.pop_back();
		else break;
	}
	return (p.back().y-y)/(x-p.back().x);
}

int Y,N,P[505],V[505];

int main()
{
	freopen ("input.txt","r",stdin);
	freopen ("output.txt","w",stdout);

	int Test; scanf ("%d",&Test); for (int Case=1;Case<=Test;Case++){
		printf ("Case #%d: ",Case);
		scanf ("%d %d",&Y,&N);
		for (int i=0;i<N;i++) scanf ("%d",&P[i]);
		
		vector<point> neg,pos;
		for (int i=0;i<N;i++){
			int v; scanf ("%d",&v);
			if (P[i] < 0) neg.push_back(point(v,-P[i]));
			else pos.push_back(point(v,P[i]));
		}

		vector<point> nh,ph;
		sort(neg.begin(),neg.end());
		sort(pos.begin(),pos.end());
		for (int i=1;i<neg.size();i++) if (neg[i-1].x == neg[i].x){
			neg.erase(neg.begin()+i); i--;
		}
		for (int i=1;i<pos.size();i++) if (pos[i-1].x == pos[i].x){
			pos.erase(pos.begin()+i); i--;
		}
		for (auto p : neg) nh = push(nh,p);
		for (auto p : pos) ph = push(ph,p);
		
		if (neg.empty()) printf ("%.12lf\n",time(ph,Y,0));
		else if (pos.empty()) printf ("%.12lf\n",time(nh,Y,0));
		else{
			double pt = time(ph,Y,0);
			double nt = time(nh,Y,0);
			vector<point> nph,pnh;
			for (auto p : neg) nph = push(nph,point(p.x,p.y+p.x*pt));
			for (auto p : pos) pnh = push(pnh,point(p.x,p.y+p.x*nt));
			double npt = time(nph,Y,-Y*pt);
			double pnt = time(pnh,Y,-Y*nt);
			printf("%.12lf\n",min(pt+npt,nt+pnt));
		}
	}

	return 0;
}