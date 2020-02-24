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
// >>Code Template<< (Multi-Thread Solver)

#undef cout
#include <windows.h>
#include <process.h>

static const int THREAD_NUM = 4;
volatile int     g_id;
int              g_nCase;
CRITICAL_SECTION g_cs;
vector<string>   g_output;

unsigned __stdcall thread_main( void* t_id ) {
	for(;;) {
		EnterCriticalSection(&g_cs);
		const int id = ++g_id;
		if(id>g_nCase) { LeaveCriticalSection(&g_cs); break; }
		cerr << setw(4) << id << " @ " << (int)t_id << " start" << endl;

		ostringstream ss;
		ss << "Case #" << id << ": ";
		case_main( ss );

		EnterCriticalSection(&g_cs);
		if(g_output.size()<id) g_output.resize(id);
		g_output[id-1] = ss.str();
		cerr << setw(4) << id << " @ " << (int)t_id << " end" << endl;
		LeaveCriticalSection(&g_cs);
	}
	return 0;
}

void END_OF_INPUT_FOR_THIS_TEST_CASE() { LeaveCriticalSection(&g_cs); }

int main() {
	cin >> g_nCase;

	InitializeCriticalSection(&g_cs);
	vector<HANDLE> thread;
	for(int i=0; i<THREAD_NUM; ++i)
		thread.push_back( (HANDLE)_beginthreadex(0, 0, &thread_main, (void*)i, 0, 0) );
	WaitForMultipleObjects( thread.size(), &thread[0], TRUE, INFINITE );
	DeleteCriticalSection(&g_cs);

	copy( g_output.begin(), g_output.end(), ostream_iterator<string>(cout) );
}
