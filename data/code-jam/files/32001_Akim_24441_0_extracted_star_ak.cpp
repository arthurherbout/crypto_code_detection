#include <cstdio>
#define eps 1e-9
#include <iostream>
using namespace std;
#define MAXN 1000

int n;
int x[MAXN], y[MAXN], z[MAXN], p[MAXN];
double omax[MAXN];
double q;

double abs(double a){
	return (a<0?-a:a);
}

int zog;

bool can(double xmin, double ymin, double zmin, double xmax, double ymax, double zmax, int wh){
	zog++;
	if (xmax-xmin<eps) return true;
	if (ymax-ymin<eps) return true;
	if (zmax-zmin<eps) return true;
	bool exist = true;
	double tmp;
	for (int i = 0; i < n; i++){
		tmp = abs((xmin+xmax)/2-x[i])+abs((ymin+ymax)/2-y[i])+abs((zmin+zmax)/2-z[i]);
		if (tmp>omax[i]) exist = false;

		tmp = 0;
		if (xmin>x[i]) tmp += xmin - x[i];
		if (xmax<x[i]) tmp += x[i] - xmax;
		if (ymin>y[i]) tmp += ymin - y[i];
		if (ymax<y[i]) tmp += y[i] - ymax;
		if (zmin>z[i]) tmp += zmin - z[i];
		if (zmax<z[i]) tmp += z[i] - zmax;
		if (tmp>omax[i]) return false;

	}
	if (exist) return true;

	if (wh==0){
		tmp = (xmin+xmax)/2;
		return can(xmin,ymin,zmin,tmp,ymax,zmax,1)||((zog<1000000)&&can(tmp,ymin,zmin,xmax,ymax,zmax,1));
	}
	if (wh==1){
		tmp = (ymin+ymax)/2;
		return can(xmin,ymin,zmin,xmax,tmp,zmax,2)||((zog<1000000)&&can(xmin,tmp,zmin,xmax,ymax,zmax,2));
	}
	if (wh==2){
		tmp = (zmin+zmax)/2;
		return can(xmin,ymin,zmin,xmax,ymax,tmp,0)||((zog<1000000)&&can(xmin,ymin,tmp,xmax,ymax,zmax,0));
	}

	if ((xmax-xmin)*(ymax-ymin)*(zmax-zmin)<1e-10) return false;
}

bool can(double Q){
	q = Q;
	for (int i = 0; i < n; i++) omax[i] = q * p[i];
	return can(0, 0, 0, 1000000, 1000000, 1000000, 0);
}

int main(){
	freopen("star.in", "rt", stdin);
	freopen("star.out", "wt", stdout);
	int T;
	scanf("%d", &T);
	for (int t = 1; t <= T; t++){
		scanf("%d", &n);
		for (int i = 0; i < n; i++) scanf("%d%d%d%d", &x[i], &y[i], &z[i], &p[i]);
		double min = 0;
		double max = 1000000000;
		while (max-min>1e-7){
			double tmp = (min+max)/2;
			cerr<<"test "<<t<<": "<<"try "<<tmp<<" diff == "<<(max-min)<<endl;
			zog = 0;
			if (can(tmp)) max = tmp; else min = tmp;
			cerr<<"zog = "<<zog<<endl;
		}
		printf("Case #%d: %.10lf\n", t, (min+max)/2);
		cerr<<min<<" "<<max<<endl;
	}
	fclose(stdin);
	fclose(stdout);
	return 0;
}
