#include <algorithm>
#include <cstdio>
#include <string>
#include <vector>
#include <cmath>
#include <iostream>
#include <map>

using namespace std;

vector<pair<pair<int,int>,int> > T;

vector<int> A[2];

int main()
{
	FILE *fi, *fo;
	fi = fopen("p1.in", "rt");
	fo = fopen("p1.out", "wt");

	int n,i,j;
	fscanf(fi, "%d", &n);
	for(i = 0; i < n; ++i)
	{
		T.clear();
		A[0].clear();
		A[1].clear();
		int t, na, nb;
		fscanf(fi, "%d%d%d", &t, &na, &nb);
		for(j=0; j<na; ++j)
		{
			char t1[10], t2[10];
			fscanf(fi, "%s%s", t1, t2);
			int it1 = ((t1[0] - '0')*10 + (t1[1] - '0'))* 60 + ((t1[3] - '0')*10 + (t1[4] - '0'));
			int it2 = ((t2[0] - '0')*10 + (t2[1] - '0'))* 60 + ((t2[3] - '0')*10 + (t2[4] - '0'));
			T.push_back(make_pair(make_pair(it1,it2), 0));
		}
		for(j=0; j<nb; ++j)
		{
			char t1[10], t2[10];
			fscanf(fi, "%s%s", t1, t2);
			int it1 = ((t1[0] - '0')*10 + (t1[1] - '0'))* 60 + ((t1[3] - '0')*10 + (t1[4] - '0'));
			int it2 = ((t2[0] - '0')*10 + (t2[1] - '0'))* 60 + ((t2[3] - '0')*10 + (t2[4] - '0'));
			T.push_back(make_pair(make_pair(it1,it2), 1));
		}
		sort(T.begin(), T.end());
		int r[2];
		memset(r,0,sizeof(r));
		for(j = 0; j<T.size(); ++j)
		{
			if (A[T[j].second].size() == 0 || A[T[j].second][0] > T[j].first.first)
			{
				r[T[j].second]++;
			}
			else
			{
				pop_heap(A[T[j].second].begin(), A[T[j].second].end(), greater<int>());
				A[T[j].second].pop_back();
			}
			A[1-T[j].second].push_back(T[j].first.second + t);
			push_heap(A[1-T[j].second].begin(), A[1-T[j].second].end(), greater<int>());
		}
		fprintf(fo, "Case #%d: %d %d\n", i+1, r[0], r[1]);
	}


	fclose(fi);
	fclose(fo);
	return 0;
}