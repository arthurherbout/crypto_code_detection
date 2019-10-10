#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
//#include <utility>
//#include <set>
//#include <map>
//#include <queue>
#include <iomanip>
using namespace std;

#define mset(A,B) memset(A,B,sizeof(A));
#define mcpy(A,B) memcpy(A,B,sizeof(B));
typedef long long ll;
typedef long double ld;
typedef vector<int> vint;
//typedef vector<string> vstr;
#define FI(I,L,U) for (int I=L;I<U;I++)
#define sqr(x) ((x)*(x))

int w,h;
int dx[2], dy[2];
bool b[110][110];
int ans;

void visit(int x, int y)
{
	b[x][y] = true;
	ans++;
	FI(i,0,2)
	{
		int x1 = x+dx[i], y1 = y+dy[i];
		if (x1 >= 0 && x1 < w && y1 >= 0 && y1 < h && !b[x1][y1])
			visit(x1,y1);
	}
}

int main()
{
	int tcase = 0;
	ifstream fin("z.in");
	ofstream fout("z.out");
	fin >> tcase;
	for (int tind = 1; tind <= tcase; tind++)
	{
		//istringstream strin();
		mset(b, 0);
		fin >> w >> h >> dx[0] >> dy[0] >> dx[1] >> dy[1];
		int x,y;
		fin >> x >> y;
		ans = 0;
		visit(x, y);
		
		fout << "Case #" << tind << ": " << ans << endl;
	}
	return 0;
}
