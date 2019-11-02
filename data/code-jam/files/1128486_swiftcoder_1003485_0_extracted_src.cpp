#include <iostream>

using namespace std;

void main()
{
	int T, L, t, N, C;
	int a[1000];
	int booster_used[2];
	cin >> T;
	
	for(int i=0; i<T; i++){
		cin >> L;
		cin >> t;
		cin >> N;
		cin >> C;
		
		for(int j=0; j <C; j++) {
			cin >> a[j];
		}
		int time = 0;
		for(int k=0; k<L; k++) {
			booster_used[k] = 0;
		}
		for(int j=0; j <N; j++) {
			int inc = 2*a[j%C];
			time += inc;
			int boost_used_for;
			if (time > t)
				boost_used_for = min(inc, time-t);


			for(int k=0; k<L; k++) {
				if (boost_used_for > booster_used[k]) {
					time -= (boost_used_for - booster_used[k])/2;
					booster_used[k]=boost_used_for;
					break;
				}
			}
			//cout << time << ",";

		}
		//cout << endl;
		cout << "Case #"<< i+1 << ": " <<time <<endl;
	}

}
