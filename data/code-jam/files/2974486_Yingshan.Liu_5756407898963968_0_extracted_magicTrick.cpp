#include <iostream>
#include <vector>

using namespace std; 

vector< vector<int> > grid1(4), grid2(4);

void work(int c) {
	cout << "Case #" << c << ": ";
	int row1, row2;
	cin >> row1;
	row1--;
	for (int i=0; i<4; i++) 
		for (int j=0; j<4; j++)
			cin >> grid1[i][j];
	cin >> row2;
	row2--;
	for (int i=0; i<4; i++) 
		for (int j=0; j<4; j++)
			cin >> grid2[i][j];
			
	int num=0, ans; 
	for (int i=0; i<4; i++) {
		for (int j=0; j<4; j++) {
			if (grid1[row1][i] == grid2[row2][j]) {
				num++;
				ans = grid1[row1][i];
			}
		} 
	}
	if (num > 1) {
		cout << "Bad magician!" << endl;	
	} else if (num == 0) {
		cout << "Volunteer cheated!" << endl;
	} else {
		cout << ans << endl;
	}
}

int main() {
	freopen("A-small-attempt0.in", "r", stdin);
	freopen("A-small-attempt0.out", "w", stdout);
	
	vector<int> temp(4);
	for(int i=0; i<4; i++) {
		grid1[i] = temp; 
		grid2[i] = temp;
	}
	
	int n;
	cin >> n;
	for (int i=0; i<n; i++) {
		work(i+1);
	}
}