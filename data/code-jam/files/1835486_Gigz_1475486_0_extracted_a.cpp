#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
#include <cstdio>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>

using namespace std;

#define FOR(i,n) for((i) = 0; (i) < (n); ++(i))
#define SFOR(i,m,n) for((i) = (m); (i) < (n); ++(i))

FILE *fi, *fo;

vector<pair<pair<int, int>, int> > A;

bool cmp(const pair<pair<int, int>, int>& a, const pair<pair<int, int>, int>& b)
{
	int q = a.first.first * b.first.second;
	int w = b.first.first * a.first.second;
	return (q < w) || (q == w && q != 0 && a.first.first < b.first.first) || (q == w && a.first.first == b.first.first && a.second < b.second);
}


void mainImpl(int _c)
{
	int n, i;
	fscanf(fi, "%d", &n);
	A.resize(n);
	FOR(i, n) fscanf(fi, "%d", &A[i].first.first);
	FOR(i, n) fscanf(fi, "%d", &A[i].first.second);
	FOR(i, n) A[i].second = i;
	sort(A.begin(), A.end(), cmp);
	fprintf(fo, "Case #%d:", _c);
	FOR(i, n) fprintf(fo, " %d", A[i].second);
	fprintf(fo, "\n");
}


int main()
{
	fi = fopen("a.in", "rt");
	fo = fopen("a.out", "wt");

	int t, tt;
	fscanf(fi, "%d", &tt);

	FOR(t, tt) mainImpl(t + 1);

	fclose(fo);
	fclose(fi);
	return 0;
}