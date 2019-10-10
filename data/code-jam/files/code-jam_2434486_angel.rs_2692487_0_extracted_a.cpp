#include <map>
#include <set>
#include <queue>
#include <vector>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <iostream>
#include <time.h>

#define fi(n) for(int i=0;i<n;i++)
#define fj(n) for(int j=0;j<n;j++)
#define fk(n) for(int k=0;k<n;k++)
#define iter(c,it) for(typeof((c).begin()) it=(c).begin();it!=(c).end();++it)
#define k_exists(c,k) ((c).count(k) > 0)
#define sort_asc(c) sort((c).begin(), (c).end())
#define sort_desc(c) sort((c).begin(), (c).end(), greater<typeof(c[0])>())

using namespace std;

int main() {
	int TC, tc;

	cin >> TC;

	for(tc=1;tc<=TC;tc++) {
		long long mote;
		long long moteb;
		int n;
		long moves_1 = 0;
		long moves_2 = 0;
		vector<long long> motes;

		cin >> mote;
		cin >> n;

		moteb = mote;

		fi(n) {
			long long m;
                        cin >> m;
                        motes.push_back(m);
		}

		sort_asc(motes);

		int i = 0;

		while(i < n) {
			if(mote > motes[i]) {
				mote += motes[i];
				i++;
			} else if(mote == 1) {
				moves_1++;
				i++;
			} else if(i == (n-1)) {
				moves_1++;
				i++;
			} else {
				mote = (mote*2)-1;
				moves_1++;
			}
		}

		i = 0;

		while(i < n) {
			if(moteb > motes[i]) {
				moteb += motes[i];
			} else {
				moves_2++;
			}

			i++;
		}

		if(moves_1 < moves_2) {
			cout << "Case #" << tc << ": " << moves_1 << endl;
		} else {
			cout << "Case #" << tc << ": " << moves_2 << endl;
		}
	}

	return 0;
}
