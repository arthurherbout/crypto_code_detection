#include <string>
#include <iostream>
#include <vector>
#include<algorithm>
#include <fstream>
#define P(x,y) make_pair(x,y)
using namespace std;
const int MX = (1 << 20);

string str[13] = { "ZERO", "ONE", "TWO", "THREE", "FOUR", "FIVE", "SIX", "SEVEN", "EIGHT", "NINE" };
int num[12] = { 0, 2, 8, 3, 6, 7, 4, 5, 9, 1 };

char letters[15] = "ZWGHXSUVIO";
int n, T, index = 0, rem;
int arr2[300];

void foo(int x){
	for (int j = 0; j<str[x].size(); j++){
		arr2[str[x][j]]--;
		if (arr2[str[x][j]] == 0) rem--;
	}
}
int main(){

	ifstream in("A-small-attempt0.in");
	ofstream out("out.txt");
	in >> T;
	while (T--){
		out << "Case #" << index+1 << ": ";
		++index;
		string ss;
		in >> ss;
		memset(arr2, 0, sizeof(arr2));
		for (int j = 0; j<ss.size(); j++) arr2[ss[j]]++;
		rem = 0;
		for (int j = 0; j<300; j++) if (arr2[j]) rem++;
		vector < int > v;
		for (int j = 0; j<10; j++){
			while (arr2[letters[j]]){
				v.push_back(num[j]);
				foo(num[j]);
			}
		}
		sort(v.begin(), v.end());
		for (int j = 0; j<v.size(); j++) out << v[j];
		out << endl;
	}

}