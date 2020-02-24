#include <iostream>
#include <vector>
#include <cmath>
#include <set>
#include <map>
#include <queue>
#include <cstdlib>
#include <sstream>
#include <cstdio>
#include <algorithm>
using namespace std;

int n, m;
vector< vector<int> > g;

void parse(string s)
{
	int p=0;
	while (s[p] != ',') ++p;
	int a = atoi( s.substr(0, p).c_str() );
	int b = atoi( s.substr(p+1).c_str() );
	g[a].push_back(b);
	g[b].push_back(a);
}

class mycomparison
{
public:
  bool operator() (const pair< pair<int,set<int> >, int > &a, const pair< pair<int,set<int> >, int > &b) const
  {
    if ( a.first.first > b.first.first ) return 1;
	if ( a.first.first == b.first.first and a.first.second.size() < b.first.second.size() ) return 1;
	return 0;
  }
};

int c, t;
void go()
{
	priority_queue< pair< pair<int,set<int> >, int >, vector< pair< pair<int,set<int> >, int > >, mycomparison > q;
	set<int> vis;
	set<int> aux; aux.insert(0);
	for (int i = 0; i < g[0].size(); ++i) aux.insert( g[0][i] );
	
	q.push( make_pair( make_pair(0, aux), 0 ) );
	
	pair< pair<int,set<int> >, int > p;
	while ( !q.empty() )
	{
		p = q.top(); q.pop();
		
		if ( vis.count( p.second ) ) continue;
		vis.insert( p.second );
		
		//cout << p.second << ": " << p.first.first << "," << p.first.second.size() << endl;
		if ( p.second == 1 ) break;
		
		for (int i = 0; i < g[p.second].size(); ++i)
		{
			set<int> n = p.first.second;
			for (int j = 0; j < g[ g[p.second][i] ].size(); ++j)
				n.insert( g[ g[p.second][i] ][j] );
			
			//cout << "push: " << g[p.second][i] << ": " << p.first.first+1 << endl;
			q.push( make_pair( make_pair( p.first.first+1, n ), g[p.second][i] ) );
		}
	}
	
	//cout << p.first.first << " " << p.first.second.size() << endl;
	
	c = p.first.first-1;
	t = p.first.second.size() - c -1;
}

int main()
{
	int T; cin >> T;
	for (int C = 1; C <= T; ++C)
	{
		cin >> n >> m;
		g.clear(); g.resize(n);
		string s;
		for (int i = 0; i < m; ++i)
		{
			cin >> s;
			parse(s);
		}
	
		go();
		cout << "Case #" << C << ": " << c << " " << t << endl;
	}
}

