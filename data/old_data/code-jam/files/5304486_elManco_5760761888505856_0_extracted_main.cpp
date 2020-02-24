#include <bits/stdc++.h>

#define fst first
#define snd second
#define mp make_pair
#define pb push_back
#define fill(x,v) memset(x,v,sizeof(x))

using namespace std;

typedef long long ll;
typedef pair<int,int> ii;
typedef vector<int> vi;
typedef vector<ii> vii;

int main(){

	int t;
	cin >> t;
	string tab[25];
	for(int tc = 0; tc < t; tc++){
		int r,c; cin >> r >> c;
		for(int i = 0; i < r; i++)
			cin >> tab[i];
		/*
		printf("Case #%d:\n",tc+1);
		for(int i = 0; i < r; i++){
			for(int j = 0; j < c; j++)
				printf("%c",tab[i][j]);
			printf("\n");
		}*/
		for(int i = 0; i < r; i++){
			for(int j = 0; j < c; j++){
				if(tab[i][j] == '?') continue;
				char car = tab[i][j];
				for(int up = i-1; up >= 0; up--){
					if(tab[up][j] == '?') tab[up][j] = car;
					else break;
				}
				for(int dw = i+1; dw < r; dw++){
					if(tab[dw][j] == '?') tab[dw][j] = car;
					else break;
				}
			}
		}

		for(int i = 0; i < r; i++){
			for(int j = 0; j < c; j++){
				if(tab[i][j] == '?'){
					// der
					bool done = false;
					for(int k = j+1; k < c; k++){
						if(tab[i][k] != '?'){
							int dwlim = i+1;
							char car = tab[i][k];
							for(int x = i+1; x < r; x++)
								if(tab[x][k] == car) dwlim = x+1;
								else break;
							for(int ii = i; ii < dwlim; ii++)
								for(int jj = j; jj < k; jj++)
									tab[ii][jj] = car;
							
							done = true;
							break;
						}
					}
					//izq
					if(done) continue;
					for(int k = j-1; k >= 0; k--){
						if(tab[i][k] != '?'){
							int dwlim = i+1;
							char car = tab[i][k];
							for(int x = i+1; x < r; x++)
								if(tab[x][k] == car) dwlim = x+1;
								else break;
							for(int ii = i; ii < dwlim; ii++)
								for(int jj = k+1; jj <= j; jj++)
									tab[ii][jj] = car;
							
							break;
						}
					}
				}
			}
		}
		printf("Case #%d:\n",tc+1);
		for(int i = 0; i < r; i++){
			for(int j = 0; j < c; j++)
				printf("%c",tab[i][j]);
			printf("\n");
		}
		//printf("\n");
	}

	return 0;
}