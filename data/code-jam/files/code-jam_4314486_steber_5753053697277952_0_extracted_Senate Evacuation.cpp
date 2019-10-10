

//#include <iostream>
#include <fstream>

using namespace std;

ifstream cin ("input.txt");
ofstream cout ("output.txt");

int main()
{
	int N;
	cin >> N;
	
	for (int q = 1; q<=N; q++) {
		cout << "Case #" << q << ":" ;
		int num_party ;
		cin >> num_party;
		char party[26] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
		int senators[num_party];
		for (int i = 0 ; i < num_party; i++) {
			cin >> senators[i];
		}
		bool finito = false;
		while (!finito) {
			int two_party[2];
			two_party[0] = 0;
			two_party[1] = 0;
			for (int i = 0; i < num_party; i++) {
				if (senators[i] > senators[two_party[0]] ) {
					two_party[1] = two_party[0];
					two_party[0] = i;
				}
				else if (senators[i] >= senators[two_party[1]]) {
					two_party [1] = i;
				}
				if (senators[i]-1 > senators[two_party[1]] ) {
					two_party[1] = i;
				}
			}
			if (senators[two_party[0]] > 1) {
				senators[two_party[0]] --;
				cout << " " << party[two_party[0]];
			}
			else {
				finito = true;
			}
			if (senators[two_party[1]] >1) {
				senators[two_party[1]]--;
				cout << party[two_party[1]];
			}
			else {
				finito = true;
			}
		}
		if (num_party %2 == 0) {
			for (int i = 0; i < num_party; i+=2) {
				cout << " " << party[i] << party[i+1];
			}
		}
		else {
			cout << " " << party[0];
			for (int i = 1; i < num_party; i+=2) {
				cout << " " << party[i] << party[i+1];
			}
		}
		
		cout << endl;
	}
	return 0;
}

