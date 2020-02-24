#include <iostream>
#include <set>

using namespace std;

int main() {
	int t;
	cin >> t;
	for (int tcase = 1; tcase <= t; tcase++) {
		int n;
		cin >> n;
		cin.ignore();	
		set <string> set1;
		set <string> set2;
		int number = 0;
		for (int i = 0; i <n; i++) {
			string str1;
			string str2;
			getline(cin, str1, ' ');
			getline(cin, str2);	
			auto a1 = set1.find(str1);
			auto a2 = set2.find(str2);
			
			if (a1 != set1.end() && a2 != set2.end())
				number++;
			else if (set1.find(str1) == set1.end())
				set1.insert(str1);
			else if (set2.find(str2) == set2.end())
				set2.insert(str2);
		}
		printf("Case #%d: %d\n", tcase, number);
		
		
	}
	return 0;
}

