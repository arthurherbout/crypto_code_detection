#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

int main(void) {
int T;
cin>>T;
cout.precision(6);
for(int i=0;i<T;++i) {
	double result = 0;
	int N;
	double X,t,S,R;
	cin>>X>>S>>R>>t>>N;
	vector<pair<double,double> > B;
	int x1,x2,x3;
	int e=0;
	double w=0;
	for (int j=0;j<N;++j) {
		cin>>x1>>x2>>x3;
		w+=(x1-e);
		e = x2;
		B.push_back(pair<double,double>(x3+S,x2-x1));
	}
	w+=X-x2;
	B.push_back(pair<double,double>(S,w));
	sort(B.begin(),B.end());
	for (int j=0;j<B.size();++j){
		if (t>(B[j].second/(B[j].first+R-S))){
			t-=B[j].second/(B[j].first+R-S);
			B[j].first+=(R-S);
		}
		else {
			double v1,v2;
			v2 = B[j].first;
			v1 = v2+R-S;
			B.push_back(pair<double,double>(v1,t*v1));
			B[j].second -= t * v1;
			
			//double t2 = t +( (B[j].second-v1*t) / v2);
			//B[j].first = B[j].second * t2;*/
			t=0;
			break;
		}
		//++j;
	}
	for (int j=0;j<B.size();++j)
		result+=B[j].second/B[j].first;
	cout.precision(7);
	cout<<"Case #"<<i+1<<": "<<result<<'\n';
}

return 0;

}