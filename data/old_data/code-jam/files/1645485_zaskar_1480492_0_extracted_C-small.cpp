#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;

struct car {
	double p;
	int s;
	double t;
	bool operator<(const car &c2) const { return p<c2.p; }
};

int bfind(car *c,int n, double p) {
	int i0=0, i1=n-1, i; double x;
	if (p<c[0].p) return 0;
	if (p>c[n-1].p) return n;
	while (i1-i0>1) {
		i=(i0+i1)/2;
		x=c[i].p;
		if (x>=p) i1=i;
		else i0=i;
	}
	return i0;
}

void bremove(car *c, int n, int p) {
	for(int i=p+1;i<n;i++) 
		c[i-1]=c[i];
}

void binsert(car *c, int n, int p, car &nc) {
	for(int i=n;i>p;i--) 
		c[i]=c[i-1];
	c[p]=nc;
}

int main(int argc, char *argv[]) {
	ifstream fin("C-small-attempt0.in");
	ofstream fout("C-small-attemp0.out");
	int T;
	fin>>T;
	car l[10000],r[10000];
	char c; int s,p;
	for(int I=0;I<T;I++) {
		int n,nl=0,nr=0;
		fin>>n;
		for(int i=0;i<n;i++) { 
			fin>>c>>s>>p;
			if (c=='L') {
				l[nl].p=p;
				l[nl].s=s;
				nl++;
			} else {
				r[nr].p=p;
				r[nr].s=s;
				nr++;
			}
		}
		sort(l,l+nl); sort(r,r+nr);
		double lmin,rmin,t,tacum=0;
		while (true) {
			lmin=-1;rmin=-1;
			int ilmin,irmin;
			for(int i=0;i<nl-1;i++) {
				if (l[i].s-l[i+1].s) {
					t=l[i].t=(l[i+1].p-l[i].p-5)/(l[i].s-l[i+1].s);
					if (t>=0 && (lmin<0||t<lmin)) { lmin=t; ilmin=i; }
				}
			}
			for(int i=0;i<nr-1;i++) {
				if (r[i].s-r[i+1].s) {
					t=r[i].t=(r[i+1].p-r[i].p-5)/(r[i].s-r[i+1].s);
					if (t>=0 && (rmin<0||t<rmin)) { rmin=t; irmin=i; }
				}
			}
			if (lmin<0&&rmin<0) break;
			if (lmin!=-1 && (rmin==-1||rmin>lmin)) t=lmin;
			else t=rmin;
			for(int i=0;i<nl;i++) { l[i].p+=l[i].s*t; }
			for(int i=0;i<nr;i++) { r[i].p+=r[i].s*t; }
			if (t==lmin) {
				int i = bfind(r,nr,l[ilmin].p);
				if (i>0 && r[i-1].p+5>l[ilmin].p) break;
				if (i+1<nr && r[i+1].p-5<l[ilmin].p) break;
				if (l[ilmin].p-2.5<r[i].p && l[ilmin].p+2.5>r[i].p) break;
				binsert(r,nr,i,l[ilmin]); nr++;
				bremove(l,nl,ilmin); nl--;
			} else {	
				int i = bfind(l,nl,r[ilmin].p);
				if (i>0 && l[i-1].p+5>r[irmin].p) break;
				if (i+1<nl && l[i+1].p-5<r[irmin].p) break;
				if (r[irmin].p-2.5<l[i].p && r[irmin].p+2.5>l[i].p) break;
				binsert(l,nl,i,r[irmin]); nl++;
				bremove(r,nr,irmin); nr--;
			}
			tacum+=t;
		}
		cout<<"done"<<endl;
		if (lmin<0&&rmin<0)
			fout<<"Case #"<<I+1<<": Possible"<<endl;
		else
			fout<<"Case #"<<I+1<<": "<<t+tacum<<endl;
	}
	fin.close();
	fout.close();
	return 0;
}

