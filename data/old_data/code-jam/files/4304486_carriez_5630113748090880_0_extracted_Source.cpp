#define _CRT_SECURE_NO_WARNINGS
#include<vector>
#include<unordered_map>
#include<unordered_set>
#include<set>
#include<iostream>
#include<math.h>
#include<algorithm>
#include<string>
#include<queue>
#include<stdint.h>
#include<bitset>
#include<deque>
#include<list>
using namespace std;
typedef long long	ll;

bool check(vector<vector<int> > &arr, vector<vector<int> > &matrix, bool row, int level, int start){

	int n = matrix[0].size();
	if (row){
		if (level){
			int i = 0;
			for (; i < n; ++i){
				if (matrix[level][i] != -1){
					if (matrix[level][i] != arr[start][i])
						return false;
				}
				else{
					if (arr[start][i] < matrix[level - 1][i])
						return false;
				}
			}
		}
		for (int i = level; i < n; ++i){
			matrix[level][i] = arr[start][i];
		}
	}
	else{
		if (level){
			int i = 0;
			for (; i < n; ++i){
				if (matrix[i][level] != -1){
					if (matrix[i][level] != arr[start][i]){
						return false;
					}
				}
				else{
					if (arr[start][i] < matrix[i][level - 1]){
						return false;
					}
				}
			}
		}
		for (int i = level; i < n; ++i){
			matrix[i][level] = arr[start][i];
		}
	}
	return true;
}
bool helper(vector<vector<int> > &arr, vector<vector<int> > &matrix, int &row, int &col, int level, int start, bool &miss){
	int n = matrix[0].size();
	if (start >= 2 * n - 1) return true;
	int rtemp = row, ctemp = col;
	//int stemp = start;
	if (!(row &(1<<level)) && check(arr, matrix, 1, level, start)){
		row |= 1 << level;
		if (miss){
			miss = false;
			if (helper(arr, matrix, row, col, level + 1, start + 1, miss)){
				return true;
			}
			miss = true;
		}
		if (check(arr, matrix, 0, level, start + 1)){
			col |= 1 << level;
			if (helper(arr, matrix, row, col, level + 1, start + 2, miss)){
				return true;
			}
		}
		for (int i = level; i < n; ++i){
			matrix[level][i] = -1;
			matrix[i][level] = -1;
		}
	}
	row = rtemp;
	col = ctemp;

	if (!(col &(1<<level)) && check(arr, matrix, 0, level, start)){
		col |= 1 << level;
		if (miss){
			miss = false;
			if (helper(arr, matrix, row, col, level + 1, start + 1, miss)){
				return true;
			}
			miss = true;
		}
		if (check(arr, matrix, 1, level, start + 1)){
			row |= 1 << level;
			if (helper(arr, matrix, row, col, level + 1, start + 2, miss)){
				return true;
			}
		}
		for (int i = level; i < n; ++i){
			matrix[level][i] = -1;
			matrix[i][level] = -1;
		}
	}
	return false;
}

bool comp(vector<int> &v1, vector<int> &v2){
	return v1[0] < v2[0];
}
string helperB(){
	int n = 0;
	cin >> n;
	string dump;
	getline(cin, dump);
	vector<vector<int> > arr(2 * n - 1, vector<int>(n, -1));
	for (int i = 0; i < 2*n - 1; ++i){
		for (int j = 0; j < n; ++j){
			cin >> arr[i][j];
		}
	}
	sort(arr.begin(), arr.end(), comp);
	int row = 0, col = 0;
	vector<vector<int> > matrix(n, vector<int>(n, -1));
	bool miss = true;
	helper(arr, matrix, row, col, 0, 0, miss);
	string ans = "";
	for (int i = 0; i < n; i++){
		if ((row & (1 << i)) == 0){
			//matrix[i]
			for (int j = 0; j < n; j++){
				ans.append(to_string(matrix[i][j]));
				ans.push_back(' ');
			}
			return ans;
		}
	}
	for (int i = 0; i < n; i++){
		if ((col &(1 << i)) == 0){
			for (int j = 0; j < n; j++){
				ans.append(to_string(matrix[j][i]));
				ans.push_back(' ');
			}
			return ans;
		}
	}
}

//#define TEST
#define SMALL
//#define LARGE
int main() {
#ifdef TEST
	freopen("test.in", "r", stdin);
	freopen("test.out", "w", stdout);
#endif
#ifdef SMALL
	freopen("B-small-attempt0.in", "r", stdin);
	freopen("B-small-attempt0.out", "w", stdout);
#endif
#ifdef LARGE
	freopen("B-large.in", "r", stdin);
	freopen("B-large.out", "w", stdout);
#endif
	int n = 0;
	cin >> n;
	string dump;
	getline(cin, dump);
	for (int i = 1; i <= n; i++) {
		printf("Case #%d: ", i);
		cout << helperB();
		cout << endl;
	}
}