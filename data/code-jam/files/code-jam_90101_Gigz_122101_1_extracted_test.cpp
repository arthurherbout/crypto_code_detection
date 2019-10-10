#include <cstdio>
#include <iostream>
#include <set>
#include <map>
#include <sstream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <utility>

using namespace std;

#define FOR(i,n) for((i) = 0; (i) < (n); ++(i))
#define SFOR(i,m,n) for((i) = (m); (i) < (n); ++(i))

string q = "welcome to code jam";
string s;
int FF[20][600];


int doit(int a, int b)
{
	if (FF[a][b] < 0)
	{
		FF[a][b] = 0;
		if (a == 19) FF[a][b] = 1;
		int i;
		SFOR(i, b, s.length())
		{
			if (s[i] == q[a]) FF[a][b] = (FF[a][b] + doit(a+1, i+1)) % 10000;
		}
	}

	return FF[a][b];
}

int main(void)
{
	FILE *fi, *fo;	
	fi = fopen("d:\\a.in", "rt");
	fo = fopen("a.out", "wt");

	
	int t, tt;
	fscanf(fi, "%d\n", &tt);

	FOR(t, tt)
	{
		int n,m,i,j;
		char buf[1000];
		fgets(buf, 1000, fi);
		s = string(buf);
		memset(FF, -1, sizeof(FF));
		int res = doit(0, 0);
		fprintf(fo, "Case #%d: ", t+1);
		if (res < 1000) fprintf(fo, "0");
		if (res < 100) fprintf(fo, "0");
		if (res < 10) fprintf(fo, "0");
		fprintf(fo, "%d\n", res);
	}

	fclose(fi);
	fclose(fo);

	return 0;
}