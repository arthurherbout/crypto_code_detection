// FileFixIt.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <cstring>
#include <string.h>
#include <map>
#include <deque>
#include <queue>
#include <stack>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <vector>
#include <set>
#include <complex>
#include <list>

using namespace std;

//#define SMALL
#define LARGE

set<string> dirSet;

int makeDir(string s)
{
	if (s.compare("/") == 0 || s.size() == 0)
		return 0;
	if (dirSet.find(s) != dirSet.end())
		return 0;
	dirSet.insert(s);
	size_t pos = s.find_last_of('/');
	string p = s.substr(0, pos);
	return makeDir(p)+1;
}

int main(int argc, char* argv[])
{
	//freopen("a.txt", "rt", stdin);
#ifdef SMALL
	freopen("A-small-attempt0.in","rt",stdin);
	freopen("A-small.out","wt",stdout);
#endif
#ifdef LARGE
	freopen("A-large.in","rt",stdin);
	freopen("A-large.out","wt",stdout);
#endif

	int T;
	scanf("%d", &T);
	for (int TC=1; TC<=T; TC++)
	{
		printf("Case #%d: ", TC);
		int M,N;
		scanf("%d%d", &M, &N);
		dirSet.clear();
		for(int i=0; i<M; i++)
		{
			string s;
			cin >> s;
			dirSet.insert(s);
		}
		int total = 0;
		for (int i=0; i<N; i++)
		{
			string s;
			cin >> s;
			total += makeDir(s);
		}
		cout << total << endl;
	}
	return 0;
}

