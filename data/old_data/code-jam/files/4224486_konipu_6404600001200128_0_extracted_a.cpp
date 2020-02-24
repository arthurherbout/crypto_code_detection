#include<bits/stdc++.h>
using namespace std;
int main () {
	int t;
	cin >> t;
	for(int c=1;c<=t;c++){
		vector<int> m;
		int n;
		cin >> n;
		for(int i=0;i<n;i++){
			int t;
			cin >> t;
			m.push_back(t);
		}
		int ans1,ans2;
		int mm=0;
		ans1=ans2=0;
		for(int i=0;i<n-1;i++){	
			if(m[i]>m[i+1]){
				ans1+=m[i]-m[i+1];
				mm=max(mm,(m[i]-m[i+1]));
			}
		}
		int total=0;
		for(int i=0;i<n-1;i++){
			ans2+=min(m[i],mm);
		}
		cout<<"Case #"<<c<<": "<<ans1<<" "<<ans2<<endl;
	}
	return 0;
}
