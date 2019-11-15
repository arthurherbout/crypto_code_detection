#include <stdio.h>
#include <algorithm>
#include <vector>
using namespace std;
#define INF 99999999.0
char LR[50];
int P[50];
int S[50];

vector< pair<double, int> > L, R, LRT;
int main(){
	freopen("C-small-attempt1.in","r",stdin);
	freopen("C-small-attempt1.out","w",stdout);
	int T;
	scanf("%d",&T);
	while(T--> 0){
		int n;
		scanf("%d", &n);
		int i, j;
		L.clear();
		R.clear();
		for(i=0;i<n;i++){
			char cc[10];
			scanf("%s %d %d",cc, &S[i], &P[i]);
			LR[i] = cc[0];
			if (LR[i] == 'L') L.push_back(make_pair((double)P[i], S[i]));
			if (LR[i] == 'R') R.push_back(make_pair((double)P[i], S[i]));
		}
		sort(L.begin() ,L.end());
		sort(R.begin(), R.end());

		double sss = 0.0;
		while (1) {
			for(i=0;i<L.size()-1;i++){
				if (L[i].second > L[i+1].second) break;
			}
			if (i >= L.size()-1) {
				for(i=0;i<R.size()-1;i++){
					if (R[i].second > R[i+1].second) break;
				}
				if (i >= R.size() - 1){ sss = -1.0; break;}
			}

			int x, y;
			double xx, yy, zz, mz = INF;
			double xxx, yyy, zzz;

			double sz;

			int type = -1, tut;
			int ii, jj;
			sz = INF;

			for(tut = 0;tut < 2;tut ++) {

			for(i=0;i<L.size();i++){
				x = i-1; y = i+1;
				if (0 <= x && x < L.size()) {
					if (L[x].second > L[i].second) {
						xx = (L[i].first - 5.0 - L[x].first)/((double)( L[x].second - L[i].second ));
					}else xx = INF;
				}
				else xx = INF;
				
				if (0 <= y && y < L.size()) {
					if (L[y].second < L[i].second) {
						yy = (L[y].first - 5.0 - L[i].first)/((double)( L[i].second - L[y].second ));
					}else yy = INF;
				}
				else yy = INF;

				zz = min(xx, yy); // collision time...

				mz = min(zz, mz);
				for(j=0;j<R.size() + 1;j++){
					x = j-1; y = j;
					double s, e;
					s = 0.0; e = INF;
					if (0 <= x && x < R.size()) {
						if (L[i].first <= R[x].first + 5.0) {
							if (L[i].second > R[x].second) {
								s = max(s, (R[x].first + 5.0 - L[i].first) / ((double)(L[i].second - R[x].second)));
							}
							else s = INF;
						}
						else { // L[i].first >= R[x].first + 5.0
							if (L[i].second < R[x].second) {
								e = min(e, (L[i].first - (R[x].first + 5.0)) / ((double)(R[x].second - L[i].second)));
							}
						}
					}
					
					if (0 <= y && y < R.size()) {
						if (L[i].first >= R[y].first - 5.0) {
							if (L[i].second < R[y].second) {
								s = max(s, (L[i].first - (R[y].first - 5.0)) / ((double)(R[y].second - L[i].second)));
							} else {
								s = INF;
							}
						}
						else { // L[i].first <= R[y].first - 5.0
							if (L[i].second > R[y].second) {
								e = min(e, ((R[y].first - 5.0) - L[i].first) / ((double)(L[i].second - R[y].second)));
							}
						}
					}

					if (s > e) continue;
					double X, Y, Z;
					if (0 <= x && x < R.size()) X = (double)R[x].first + s * R[x].second;
					else X = -INF;

					Y = (double)L[i].first + s * L[i].second;
					
					if (0 <= y && y < R.size()) Z = (double)R[y].first + s * R[y].second;
					else Z = -INF;


					xxx = yyy = INF;
					if (0 <=x && x < R.size())
					if (L[i].second < R[x].second) {
						xxx = (Y-X-5.0)/(R[x].second - L[i].second);
					}
					if (0 <=y && y < R.size())
					if (L[i].second > R[y].second) {
						yyy = (Z-Y-5.0)/(L[i].second - R[y].second);
					}

					zzz = min(xxx, yyy);

					if (zzz > zz) {
						if (sz > s) {
							sz = s;
							type = tut;
							ii = i;
							jj = j;
						}
					}
				}
			}

			LRT = L;
			L = R;
			R = LRT;

			}


			if (sz <= mz) {
				sss += sz;
				for(i=0;i<L.size();i++) L[i].first += (double)L[i].second * sz;
				for(i=0;i<R.size();i++) R[i].first += (double)R[i].second * sz;

				if (type == 0) {
					R.push_back(L[ii]);
					sort(R.begin() ,R.end());
					vector<pair<double,int> >::iterator vp = L.begin();
					for(i=0;i<ii;i++) vp ++;
					L.erase(vp);
				}
				else {
					L.push_back(R[ii]);
					sort(L.begin() ,L.end());
					vector<pair<double,int> >::iterator vp = R.begin();
					for(i=0;i<ii;i++) vp ++;
					R.erase(vp);
				}
			}
			else{
				sss += mz;
				break;
			}

		}
		static int cs = 1;
		printf("Case #%d: ", cs++);
		if(sss < -0.5 )printf("Possible\n");
		else printf("%.9lf\n", sss);
	}
	return 0;
}