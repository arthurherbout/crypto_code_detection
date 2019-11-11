#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

int main(void) {
	int T;
	cin>>T;
	for (int i=0;i<T;++i){
		unsigned long long L,t,N,C;
		cin>>L>>t>>N>>C;
		vector<int> a;
		vector<long long unsigned> time;		
		vector<long long unsigned> duration;
		int a1;
		for (int j=0;j<C;++j){
			cin>>a1;
			a.push_back(a1);
		}
		time.push_back(0);
		for (int j=1;j<N+1;++j) {
			unsigned long long newT = 2*a[(j-1)%C];			
			unsigned long long newT2 = time[j-1] + newT;
			time.push_back(newT2);
			duration.push_back(newT);
		}
		int first;
		for (first=1;first<N+1;++first){
			if (time[first]>t){
				if (time[first-1] != t && time[first]!=t){
					time.insert(time.begin()+first,t);
					duration.insert(duration.begin()+first-1,0);
					duration[first-1] =time[first] - time[first-1];
					duration[first] = time[first+1] - time[first];
					++first;
				}
				break;
			}
		}
		sort(duration.begin()+first-1,duration.end());
		int n = duration.size()-(first-1);
		for (int j=0;j<L && j<n ;++j) {
			duration[duration.size()-1-j]/=2;
		}
		unsigned long long ans=0;
		for (int j=0;j<duration.size();++j)
			ans+=duration[j];
		cout<<"Case #"<<i+1<<": "<<ans<<'\n';
	}
	return 0;
}

int main1(void) {
	int T;
	cin>>T;
	for (int i=0;i<T;++i){
		int R,C;
		cin>>R>>C;
		char c;
		vector<string> ans;
		for (int j=0;j<R;++j) {
			string s;
			cin>>s;
			ans.push_back(s);
		}
		bool impossible =  false;
		for (int j=0;j<R;++j) {
			if (impossible)
				break;
			for (int k=0;k<C;++k) {
				if (ans[j][k]=='#') {
					if (j==(ans.size()-1) || (k==ans[j].length()-1) || ans[j+1][k]!='#' || ans[j][k+1]!='#' || ans[j+1][k+1] !='#') {
						impossible = true;
						break;
					}
					ans[j][k] = '/';
					ans[j+1][k]= '\\';
					ans[j][k+1] = '\\';
					ans[j+1][k+1] = '/';
				}
			}
		}
		cout<<"Case #"<<i+1<<":\n";
		if (impossible)
			cout<<"Impossible\n";
		else {
			for(int j=0;j<R;++j)
				cout<<ans[j]<<'\n';
		}
	}
	return 0;
}