#include <iostream>
#include <cmath>
using namespace std;

int x, cost;
int grid[4][4];

bool orrow(int i, int j){
	bool dirty = false;
	for(int k = 0; k < x; k++){
		if(grid[i][k] || grid[j][k]){
			if(grid[i][k] == 0){
				grid[i][k] = 1;
				cost++;
				dirty = true;
			}
			if(grid[j][k] == 0){
				grid[j][k] = 1;
				cost++;
				dirty = true;
			}
		}
	}
	return dirty;
}

bool orcol(int i, int j){
	bool dirty = false;
	for(int k = 0; k < x; k++){
		if(grid[k][i] || grid[k][j]){
			if(grid[k][i] == 0){
				grid[k][i] = 1;
				cost++;
				dirty = true;
			}
			if(grid[k][j] == 0){
				grid[k][j] = 1;
				cost++;
				dirty = true;
			}
		}
	}
	return dirty;
}
int sumrow(int i){
	int res = 0;
	for(int k = 0; k < x; k++) res += grid[i][k];
	return res;
}

int sumcol(int i){
	int res = 0;
	for(int k = 0; k < x; k++) res += grid[k][i];
	return res;
}

int maxsumrow(){
	int res = -1;
	int max = -1;
	for(int i = 0; i < x; i++){
		if(sumrow(i) > max){
			res = i;
			max = sumrow(i);
		}
	}
	return res;
}

int maxsumcol(){
	int res = -1;
	int max = -1;
	for(int i = 0; i < x; i++){
		if(sumcol(i) > max){
			res = i;
			max = sumcol(i);
		}
	}
	return res;
}
int main (){
	int ncases;
	cin >> ncases;
	for(int i = 1; i <= ncases; i++){
		cost = 0;
		cin >> x;
//		cout << x << endl;
		char cgrid[4][4];
		for(int j = 0; j < x; j++){
			for(int k = 0; k < x; k++){
				cin >>cgrid[j][k];
				grid[j][k] = cgrid[j][k] - '0';
			}
		}

		bool dirty = true;	
		while(dirty){
			dirty = false;
			for(int j = 0; j < x; j++){
				for(int k = 0; k < x; k++){
					for(int l = j+1; l < x; l++){
						if(grid[j][k] && grid[l][k]){
							bool res = orrow(j,l);
							if(res) dirty = true;

						}
					}
				}
			}

			for(int j = 0; j < x; j++){
				for(int k = 0; k < x; k++){
					if(sumrow(j) == 0 && sumcol(k) == 0){
						grid[j][k] = 1;
						dirty = true;
						cost++;
					}

				}
			}

			for(int j = 0; j < x; j++){
				if(sumrow(j) == 0){
					bool res = orrow(j,maxsumrow());
					if(res) dirty = true;
				}
			}
			for(int j = 0; j < x; j++){
				if(sumcol(j) == 0){
					bool res = orcol(j,maxsumcol());
					if(res) dirty = true;
				}
			}
		}


		/*for(int j = 0; j < x; j++){
			for(int k = 0; k < x; k++){
				cout <<grid[j][k];
			}
			cout << endl;
		}
		cout << endl;
	*/	
		cout << "Case #" << i << ": " << cost << endl;

	}

		
  
	return 0;
}


