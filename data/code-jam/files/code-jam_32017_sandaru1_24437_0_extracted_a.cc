#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <map>

using namespace std;

string itoa(int val) {stringstream ss;ss << val;return ss.str();}
typedef vector<int> vi;
vi parseInt(string s) {stringstream ss(s);vi ans;while (!ss.eof()) {int temp; ss >> temp; ans.push_back(temp); } return ans;}
#define COPY(x,y) y.resize(x.size());copy(x.begin(),x.end(),y.begin())
#define pb push_back
#define SWAP(t,x,y) t temp=x;x=y;y=temp;
#define ll long long

ll t,n,a,b,c,d,x0,y0,m;
vector<ll> px;
vector<ll> py;

int main() {
	cin >> t;
	for(int q=0;q<t;q++) {
		cin >> n >> a >> b >> c >> d >> x0 >> y0 >> m;
		px.clear(); py.clear();
		ll x = x0; ll y = y0;
		px.pb(x); py.pb(y);
		for(int i=1;i<n;i++) {
			x = (a * x + b) % m;
			y = (c * y + d) % m;
			px.pb(x); py.pb(y);
		}
		ll ans = 0;
		for(int i=0;i<n;i++)
			for(int k=i+1;k<n;k++)
				for(int j=k+1;j<n;j++)
					if ((px[i]+px[k]+px[j])%3==0 && (py[i]+py[k]+py[j])%3==0)
						ans++;
		cout << "Case #" << q+1 << ": " << ans << endl;
	}
	return 0;
}
