using namespace std;
#include <map>
#include <set>
#include <algorithm>
#include <cstdio>
const int MAXN = 30;

int n, m;
int s[MAXN][MAXN], w[MAXN][MAXN], t[MAXN][MAXN];

struct vertex{
	int i, j, side;
	friend bool operator <(const vertex &a, const vertex &b){
		if (a.i != b.i) return a.i < b.i;
		if (a.j != b.j) return a.j < b.j;
		if (a.side != b.side) return a.side < b.side;
		return false;
	}
	vertex(int a = 0, int b = 0, int c = 0){
		i = a;
		j = b;
		side = c;
	}
};

int get_time(vertex a, vertex b, int timeinit){
	int s1 = a.side;
	int s2 = b.side;
	bool EW = ((min(s1, s2) == 1) && (max(s1, s2) == 2)) || ((min(s1, s2) == 3) && (max(s1, s2) == 4));
	int dt = (timeinit - t[a.i][a.j]);
	int res = timeinit;
	if (EW){
		if (dt % (s[a.i][a.j] + w[a.i][a.j]) >= s[a.i][a.j])
		;
		else
			res += s[a.i][a.j] - dt % (s[a.i][a.j] + w[a.i][a.j]);
	}
	else{
		if (dt % (s[a.i][a.j] + w[a.i][a.j]) < s[a.i][a.j])
		;
		else{
			res += s[a.i][a.j] + w[a.i][a.j] - dt % (s[a.i][a.j] + w[a.i][a.j]);
		}
	}
	return res + 1;
}

set<pair<int, vertex> > hp;
map<vertex, int> dst;

void relax(vertex a, int t){
	if (a.i < 0) return;
	if (a.j < 0) return;
	if (a.i >= n) return;
	if (a.j >= m) return;
	if (dst.count(a) == 0){
		dst[a] = t;
		hp.insert(make_pair(t, a));
	}
	if (dst[a] > t){
		hp.erase(make_pair(dst[a], a));
		dst[a] = t;
		hp.insert(make_pair(t, a));
	}
}

void make_test(int testnum){
	scanf("%d%d", &n, &m);
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++){
			scanf("%d%d%d", &s[i][j], &w[i][j], &t[i][j]);
			t[i][j] = t[i][j] % (s[i][j] + w[i][j]) - s[i][j] - w[i][j];
		}
	dst.clear();
	hp.clear();
	relax(vertex(n - 1, 0, 4), 0);
	vertex b;
	printf("Case #%d: ", testnum);
	while (true){
		vertex no = (*(hp.begin())).second;
		if ((no.i == 0) && (no.j == m - 1) && (no.side == 2)){
			printf("%d\n", dst[no]);
			break;
		}

		hp.erase(hp.begin());

		if (no.side == 1){
		        //going
		        relax(vertex(no.i - 1, no.j    , 4), dst[no] + 2);
		        relax(vertex(no.i    , no.j - 1, 2), dst[no] + 2);

		        //crossing
		        b = vertex(no.i, no.j, 2);
       		        relax(b, get_time(no, b, dst[no]));

		        b = vertex(no.i, no.j, 4);
       		        relax(b, get_time(no, b, dst[no]));
			

		}


                 
		if (no.side == 2){
		        //going
		        relax(vertex(no.i - 1, no.j    , 3), dst[no] + 2);
		        relax(vertex(no.i    , no.j + 1, 1), dst[no] + 2);

		        //crossing
		        b = vertex(no.i, no.j, 3);
       		        relax(b, get_time(no, b, dst[no]));

		        b = vertex(no.i, no.j, 1);
       		        relax(b, get_time(no, b, dst[no]));
		}



		if (no.side == 3){
		        //going
		        relax(vertex(no.i + 1, no.j    , 2), dst[no] + 2);
		        relax(vertex(no.i    , no.j + 1, 4), dst[no] + 2);

		        //crossing
		        b = vertex(no.i, no.j, 2);
       		        relax(b, get_time(no, b, dst[no]));

		        b = vertex(no.i, no.j, 4);
       		        relax(b, get_time(no, b, dst[no]));
		}


		if (no.side == 4){
		        //going
		        relax(vertex(no.i + 1, no.j    , 1), dst[no] + 2);
		        relax(vertex(no.i    , no.j - 1, 3), dst[no] + 2);

		        //crossing
		        b = vertex(no.i, no.j, 1);
       		        relax(b, get_time(no, b, dst[no]));

		        b = vertex(no.i, no.j, 3);
       		        relax(b, get_time(no, b, dst[no]));
		}
	}
}

int main(){
	freopen("input.txt", "rt", stdin);
	freopen("output.txt", "wt", stdout);
	int tt;
	scanf("%d", &tt);
	for (int i = 1; i <= tt; ++i)
		make_test(i);
	return 0;
}
