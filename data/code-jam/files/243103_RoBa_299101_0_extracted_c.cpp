#include <cstdio>
#include <string>
#include <vector>
#include <set>
using namespace std;

typedef long long LL;
struct Pt {int x, y, id;} pt[1024];
vector<Pt> vy[32];
vector<int> mp[1024];
int mat[1024][1024];
set<LL> dp[1024];
set<LL>::iterator itr;

int cmp_x(Pt a, Pt b) {return a.x < b.x;}
int cmp_y(Pt a, Pt b) {
	if (a.y != b.y) return a.y < b.y;
	return a.x < b.x;
}

int find(int yy, int xx) {
	int i;
	for (i = 0 ; i < vy[yy].size() ; i++)
		if (vy[yy][i].x > xx) return vy[yy][i].id;
	return -1;
}

LL encode(int *mask, int n) {
	int i;
	LL s = 0;
	for (i = n - 1 ; i >= 0 ; i--) {
		s *= 3;
		s += mask[i];
	}
	return s;
}

int main() {
	freopen("c-small-attempt4.in","r",stdin);
	freopen("c-small-attempt4.out","w",stdout);
	int T, i, n, ca = 0;
	int y, j, k;
	scanf("%d",&T);
	while (T--) {
		for (i = 0 ; i <= 30 ; i++)
			vy[i].clear();
		scanf("%d",&n);
		int maxy = 0;
		for (i = 0 ; i < n ; i++) {
			scanf("%d%d",&pt[i].x,&pt[i].y);
			maxy >?= pt[i].y;
			pt[i].id = i;
			vy[pt[i].y].push_back(pt[i]);
			mp[i].clear();
		}
		memset(mat, 0, sizeof(mat));
		int pe = 0;
		for (y = 1 ; y <= 30 ; y++) {
			for (i = 0 ; i < vy[y].size() ; i++) {
				int tmp;
					tmp = find(y-1, vy[y][i].x);
					if (tmp != -1) {
						mat[vy[y][i].id][tmp] = mat[tmp][vy[y][i].id] = 1;
						//e[pe].u = tmp; e[pe].v = vy[y][i].id;
						mp[tmp].push_back(vy[y][i].id);
						mp[vy[y][i].id].push_back(tmp);
						++pe;
					}
					tmp = find(y, vy[y][i].x);
					if (tmp != -1) {
						mat[vy[y][i].id][tmp] = mat[tmp][vy[y][i].id] = 1;
						//e[pe].u = tmp; e[pe].v = vy[y][i].id;
						mp[tmp].push_back(vy[y][i].id);
						mp[vy[y][i].id].push_back(tmp);
						++pe;
					}
					tmp = find(y+1, vy[y][i].x);
					if (tmp != -1) {
						mat[vy[y][i].id][tmp] = mat[tmp][vy[y][i].id] = 1;
						//e[pe].u = tmp; e[pe].v = vy[y][i].id;
						mp[tmp].push_back(vy[y][i].id);
						mp[vy[y][i].id].push_back(tmp);
						++pe;
					}
			}
		}
		printf("Case #%d: ",++ca);
		if (pe == 0) {printf("1\n"); continue;}
		/*
		for (i = 0 ; i < n ; i++) {
			printf("%d:",i);
			for (j = 0 ; j < mp[i].size() ; j++)
				printf(" %d",mp[i][j]);
			printf("\n");
		}
		*/
		int flg = 0;
		for (i = 0 ; i < n ; i++)
			for (j = 0 ; j < mp[i].size() ; j++)
				for (k = j + 1; k < mp[i].size() ; k++)
					if (mp[i][j] != mp[i][k] && mat[mp[i][j]][mp[i][k]]) {flg = 1; goto out;}
out:
		if (flg == 0) {printf("2\n"); continue;}

		sort(pt, pt+n, cmp_x);
		for (i = 0 ; i <= n ; i++) dp[i].clear();
		dp[0].insert(0);
		int fff[32], tk[4], tmpy, col;
		memset(fff, 0, sizeof(fff));
		for (i = 0 ; i < n ; i++) {
			//printf("%d:",i);
			for (itr = dp[i].begin() ; itr != dp[i].end() ; ++itr) {
				//printf(" %I64d", *itr);
				LL mask = *itr;
				tk[0] = tk[1] = tk[2] = 0;
				LL tmask;
				int tp, mask_arr[32];
				memset(mask_arr, 0, sizeof(mask_arr));
				for (tmask = mask , tp = 0 ; tmask ; ) {
					mask_arr[tp++] = (tmask % 3);
					tmask /= 3;
				}
				for (tmpy = pt[i].y - 1 ; tmpy <= pt[i].y + 1 ; tmpy++)
					if (fff[tmpy]) tk[mask_arr[tmpy-1]] = 1;
				for (col = 0 ; col <= 2 ; col++) {
					if (tk[col] == 0) {
						int bak = mask_arr[pt[i].y-1];
						mask_arr[pt[i].y-1] = col;
						dp[i+1].insert(encode(mask_arr, maxy));
						mask_arr[pt[i].y-1] = bak;
					}
				}
			}
			fff[pt[i].y] = 1;
			//printf("\n");
		}
		if (!dp[n].empty()) printf("3\n");
		else printf("4\n");
	}
	return 0;
}
