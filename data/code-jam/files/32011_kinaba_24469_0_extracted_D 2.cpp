//-----------------------------------------------------------------------------
// >>Code Template<< (for Visual C++)

#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <complex>
#include <cmath>
#define  cout os
using namespace std;
typedef long long LL;
void END_OF_INPUT_FOR_THIS_TEST_CASE(); // stub for multi-threading

//-----------------------------------------------------------------------------
// >>Main<<

void case_main( ostream& os )
{
	int N, M; cin>>N>>M;
	vector<string> d(N);
	for(int y=0; y<N; ++y)
		cin >> d[y];
	END_OF_INPUT_FOR_THIS_TEST_CASE();

	typedef pair<int,int> point;
	set<point> foundT;
	foundT.insert(point(0,0));
	set<point> fixed;
	map<point,int> bestDist;

	bool newT = true;
	while( newT )
	{
		newT = false;
		map<point,point> prev;
		vector<point> Q;
		map<point,int> dist;

		for(set<point>::iterator it=foundT.begin(); it!=foundT.end(); ++it)
		{
			point s = *it;
			prev[s] = s;
			Q.push_back(s);
			dist[s] = 0;
		}

		while( !Q.empty() )
		{
			vector<point> Q2;
			for(int i=0; i!=Q.size(); ++i)
			{
				point p = Q[i];
				int y = p.first;
				int x = p.second;
				int dy[]={-1,+1,0,0};
				int dx[]={0,0,-1,+1};
				for(int j=0; j!=4; ++j)
				{
					int yy=y+dy[j];
					int xx=x+dx[j];
					point pp(yy,xx);
					if( 0<=yy && yy<N && 0<=xx && xx<M && d[yy][xx]!='.' && !dist.count(pp) ) {
						prev[pp]=p, Q2.push_back(pp), dist[pp]=dist[p]+1;
						if( !fixed.count(pp) )
							if( !bestDist.count(pp) || bestDist[pp] > dist[pp] ) {
								bestDist[pp] = dist[pp];
							}

						if( d[yy][xx]=='T' && !foundT.count(pp) )
						{
							newT = true;
							foundT.insert(pp);

							fixed.insert(pp);
							for(point p=pp; prev[p]!=p; p=prev[p])
								fixed.insert(prev[p]);

							goto nextT;
						}
					}
				}
			}
			Q.swap(Q2);
		}
	nextT:;
	}

	int ans = 0;
	for(map<point,int>::iterator it=bestDist.begin(); it!=bestDist.end(); ++it)
	{
//cerr << it->first.first << "," << it->first.second << " " << it->second << endl;
//cerr << fixed.count(it->first) << " " << dist2[it->first] << endl;
		ans += it->second;
	}
	os << ans << endl;
}

//-----------------------------------------------------------------------------
// >>Code Template<< (Single-Thread Solver)

#undef cout
void END_OF_INPUT_FOR_THIS_TEST_CASE() {}
int main() {
	int nCase; cin >> nCase;
	for(int id=1; id<=nCase; ++id) {
		cout << "Case #" << id << ": ";
		case_main( cout );
	}
}
