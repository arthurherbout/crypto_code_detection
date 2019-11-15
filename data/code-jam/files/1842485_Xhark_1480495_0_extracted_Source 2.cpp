#include <stdio.h>
#include <memory.h>
#include <algorithm>
#include <vector>

using namespace std;
char dat[100][100];

int bfx[10000], bfy[10000], head;
bool bfc[100][100];

int A[100][100], B[100][100];

class POS{
public:
	int a[100][100], b[100][100];
	bool c;
};

int main(){
	//freopen("input.txt","r",stdin);

	
	freopen("D-small-attempt2.in","r",stdin);
	freopen("D-small-attempt2.out","w",stdout);
	
	int T;
	scanf("%d",&T);
	while(T-->0){
		int R, C;
		scanf("%d %d", &R,&C);
		int i, j;
		for(i=0;i<R;i++){
			scanf("%s", dat[i]);
		}
		static int cs = 1;
		printf("Case #%d:\n", cs++);
		int d;
		for(d=0;d<10;d++){
			for(i=0;i<R;i++){
				for(j=0;j<C;j++){
					if (dat[i][j] == '0' + d) {
						printf("%c: ", dat[i][j]);

						const int dx[3] = {-1, 0, 0};
						const int dy[3] = {0, 1, -1};
						int p, q, r, w, z;
						memset(bfc, 0, sizeof(bfc));
						bfx[0] = i; bfy[0] = j;
						head = 1;
						bfc[i][j] = true;
						for(p=0;p<head;p++){
							for(q=0;q<3;q++){
								int X, Y;
								X = bfx[p]+dx[q]; Y = bfy[p]+dy[q];
								if (X < 0 || X >= R || Y < 0 || Y >= C) continue;
								if (dat[X][Y] == '#' || bfc[X][Y]) continue;
								bfc[X][Y] = true;
								bfx[head] = X;
								bfy[head] = Y;
								head ++;
							}
						}
						printf("%d ", head);
						bool lcy;
						lcy = true;

						int s, e;
						s = -1; e = -1;
						for(p=0;p<C;p++){
							if (bfc[i][p]) {
								if (s == -1) s = p;
								e = p;
							}
						}

						for(w=0;w<100;w++)for(z=0;z<100;z++) A[w][z] = B[w][z] = 0;
						vector<POS> V;
						for(p=0;p<i;p++){
							for(q=0;q<C;q++){
								if (bfc[p][q]) {
									POS u;
									memset(&u, 0, sizeof(u));
									for(r=q;r<C;r++){
										if(!bfc[p][r]) break;
									}
									r --;
									
									for(w=0;w<100;w++){
										//if ((s+w > e && q+w > r) || okay) break;
										int w1, w2;
										w1 = min(e,s+w); w2 = min(r,q+w);

/*										if (dat[i+1][w1] == '#' && 
											bfc[p+1][w2]) okay = true;*/

										for(z=0;z<100;z++){
//											if ((w1-z < s && w2-z < q) || okay) break;
											int ww1, ww2;
											ww1 = max(s,w1-z); ww2 = max(q,w2-z);

											if (dat[i+1][ww1] == '#' && 
												dat[p+1][ww2] != '#' &&
												!bfc[p+1][ww2]){
													A[w][z] ++;
													u.a[w][z] = 1;
											}

											if (dat[i+1][ww1] == '#' &&
												bfc[p+1][ww2]) u.a[w][z] = 2;
										}
										
									}
									for(w=0;w<100;w++){
										//if ((e-w < s && r-w < q) || okay) break;
										int w1, w2;
										w1 = max(s,e-w); w2 = max(q,r-w);
/*										if (dat[i+1][w1] == '#' && 
											bfc[p+1][w2]) okay = true;*/

										for(z=0;z<100;z++){
//											if ((w1+z > e && w2+z > r) || okay) break;
											int ww1, ww2;
											ww1 = min(e,w1+z); ww2 = min(r,w2+z);

											if (dat[i+1][ww1] == '#' && 
												dat[p+1][ww2] != '#' &&
												!bfc[p+1][ww2]){
													B[w][z] ++;
													u.b[w][z] = 1;
											}

											if (dat[i+1][ww1] == '#' &&
												bfc[p+1][ww2]) u.b[w][z] = 2;
										}
									}
									q = r;
									V.push_back(u);
								}
							}
						}
						
						int how = 0, bhow = -1;
						while(how != bhow){
							bhow = how;
							for(p=0;p<V.size();p++){
								if (!V[p].c) {
									bool good = false;
									for(w=0;w<100;w++) for(z=0;z<100;z++){
										if (V[p].a[w][z] == 2 && A[w][z] == 0) {
											good = true;
										}
										if (V[p].b[w][z] == 2 && B[w][z] == 0) {
											good = true;
										}
									}
									if (good) {
										V[p].c = true;
										for(w=0;w<100;w++) for(z=0;z<100;z++){
											if(V[p].a[w][z] == 1) A[w][z] --;
											if(V[p].b[w][z] == 1) B[w][z] --;
										}
										how ++;
									}
								}
							}
						}
						lcy = (V.size() == how);

						printf("%s\n", lcy?"Lucky":"Unlucky");
					}
				}
			}
		}

	}
	return 0;
}