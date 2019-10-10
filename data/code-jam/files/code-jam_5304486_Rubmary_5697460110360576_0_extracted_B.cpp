#include <cstdio>
#include <iostream>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <vector>
#include <set>
#include <map>

using namespace std;

#define pb push_back
#define ri(x) scanf("%d",&x)
#define rii(x,y) ri(x),ri(y)
#define ms(obj,val) memset(obj,val,sizeof(obj))
#define ms2(obj,val,sz) memset(obj,val,sizeof(obj[0])*sz)
#define FOR(i,f,t) for(int i=f;i<(int)t;i++)
#define FORR(i,f,t) for(int i=f;i>(int)t;i--)
#define EPS 1e-28
#define PI 2*acos(0.0)
#define y1 fdsad

typedef long long ll;
typedef vector<int> vi;


const int MAXN = 60;
int T, N, P, R[MAXN], Q[MAXN][MAXN];
int open[MAXN], pend[MAXN];
struct Pt{
	ll pos;
	int t, id;
	bool operator <(Pt p)const{
		return pos < p.pos || (pos==p.pos && t<p.t) || (pos==p.pos && t==p.t && id < p.id);
	}
};
vector <Pt> Pts;
int main(){
	cin >> T;
	FOR(t, 1, T+1){
		cin >> N >> P;
		FOR(i, 0, N) cin >> R[i];
		FOR(i, 0, N) FOR(j, 0, P) cin >> Q[i][j];
		Pts.clear();
		FOR(i, 0, N){
			FOR(j, 0, P){
				int pos1, pos2;
				pos1 = (10*Q[i][j] + 11*R[i] - 1)/(11*R[i]);
				pos2 = (10*Q[i][j])/(9*R[i]);
				if(pos2 < pos1) continue;
				Pts.pb({pos1, 0, i});
				Pts.pb({pos2, 1, i});
			}
		}
		sort(Pts.begin(), Pts.end());
		ms(open, 0); ms(pend, 0);
		int cnt = 0, ans = 0;
		int S = Pts.size();

		FOR(i, 0, S){
			Pt c = Pts[i];
			if(c.t == 0){
				if(open[c.id] == 0) cnt++;
				open[c.id]++;
				if(cnt == N){
					ans++;
					FOR(j, 0, N) {
						open[j]--;
						pend[j]++;
						if(open[j]==0) cnt--;
					}
				}
			}else{
				if(pend[c.id] > 0) pend[c.id]--;
				else{
					open[c.id]--;
					if(open[c.id] == 0) cnt--;
				}
			}
		}
		
		cout << "Case #" << t << ": " << ans <<endl;
	}
	
}

