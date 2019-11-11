#include <cstdio>

using namespace std;

int main(){
	freopen("A-large.in", "r", stdin);
	freopen("out.txt", "w", stdout);
	
	int T;
	scanf("%d", &T);
	for (int tc = 1; tc <= T; ++tc){
		printf("Case #%d:\n", tc);
		int r,c;
		scanf("%d %d\n", &r, &c);
		int pic[r][c];
		char ch;
		for (int i = 0; i < r; ++i){
			for (int j = 0; j < c; ++j){
				scanf("%c", &ch);
				if (ch == '.') pic[i][j] = 0;
				if (ch == '#') pic[i][j] = 1;
			}
			scanf("\n");
		}
		bool ok = true;
		for (int i = 0; i < r && ok; ++i){
			for (int j = 0; j < c && ok; ++j){
				if (pic[i][j] == 1){
					if (i<r-1 && j<c-1 && pic[i][j+1] == 1 &&
					    pic[i+1][j] == 1 && pic[i+1][j+1] == 1){
						pic[i][j] = 2;
						pic[i][j+1] = 3;
						pic[i+1][j] = 3;
						pic[i+1][j+1] = 2;
					}
					else ok = false;
				}
			}
		}
		if (!ok) printf("Impossible\n");
		else {
			for (int i = 0; i < r; ++i){
				for (int j = 0; j < c; ++j){
					if (pic[i][j] == 0) printf(".");
					if (pic[i][j] == 2) printf("/");
					if (pic[i][j] == 3) printf("\\");
				}
				printf("\n");
			}
		}
	}
	
	return 0;
}
