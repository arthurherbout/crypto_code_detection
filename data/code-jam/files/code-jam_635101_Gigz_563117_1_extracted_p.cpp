#include <cstdio>
#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <map>
#include <set>
#include <utility>
using namespace std;

#define FOR(i,n) for(i=0; i<(n); ++i)

struct Folder
{
	string name;
	vector<Folder> children;
} *root;

vector<string> split(string s)
{
	int i;
	FOR(i, s.size()) if (s[i] == '/') s[i] = ' ';
	stringstream ss;
	ss<<s;
	vector<string> F;
	string f;
	while (ss>>f) F.push_back(f);
	return F;
}

Folder* find(Folder* f, string s)
{
	int i;
	FOR(i, f->children.size()) if (f->children[i].name == s) return &f->children[i];
	return NULL;
}

int doit(vector<string> P, int pos, Folder* f)
{
	int ret = 0;
	if (pos == P.size())
		return ret;
	if (find(f, P[pos]) == NULL)
	{
		Folder *t = new Folder();
		t->name = P[pos];
		f->children.push_back(*t);
		++ret;
	}
	return ret + doit(P, pos + 1, find(f, P[pos]));
}

int main()
{
	int t, tt;
	FILE *fi = fopen("a.in", "rt");
	FILE *fo = fopen("a.out", "wt");
	fscanf(fi, "%d", &tt);
	for (t = 0; t < tt; ++t)
	{
		root = new Folder();
		int n,m;
		fscanf(fi, "%d%d\n", &n, &m);
		int i,j;
		FOR(i, n)
		{
			char S[10001];
			fgets(S, 10001, fi);
			string s(S);
			doit(split(s), 0, root);
		}
		int res = 0;
		FOR(i, m)
		{
			char S[10001];
			fgets(S, 10001, fi);
			string s(S);
			res += doit(split(s), 0, root);
		}
		fprintf(fo, "Case #%d: %d\n", t+1, res);
	}
	fclose(fi);
	fclose(fo);
	return 0;
}