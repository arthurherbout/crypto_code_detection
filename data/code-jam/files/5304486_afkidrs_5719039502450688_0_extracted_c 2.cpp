#include <bits/stdc++.h>

using namespace std;
int t;
int hd, ad, hk, ak, b,d;
typedef pair<int,int> ii;
typedef pair<ii,ii> iii;


bool attack(int hd, int ad, int hk, int ak, set<iii>& S, set<iii>& V) {

	hk-=ad;
	if(hk<=0) return true;
	hd-=ak;
	if(hd<=0) return false;
	if(V.find(make_pair(ii(hd,ad),ii(hk,ak)))==V.end()) {
		S.insert(make_pair(ii(hd,ad),ii(hk,ak)));
	}
	return false;
}

void buff(int hd, int ad, int hk, int ak, set<iii>& S, set<iii>& V) {
	ad+=b;
	hd-=ak;
	if(hd<=0) return;
	if(V.find(make_pair(ii(hd,ad),ii(hk,ak)))==V.end()) {
		S.insert(make_pair(ii(hd,ad),ii(hk,ak)));
	}
}

void cure(int shd, int ad, int hk, int ak, set<iii>& S, set<iii>& V) {
	shd = hd;
	shd-=ak;
	if(shd<=0) return;
	if(V.find(make_pair(ii(shd,ad),ii(hk,ak)))==V.end()) {
		S.insert(make_pair(ii(shd,ad),ii(hk,ak)));
	}
}

void debuff(int hd, int ad, int hk, int ak, set<iii>& S, set<iii>& V) {
	ak=max(ak-d,0);
	hd-=ak;
	if(hd<=0) return;
	if(V.find(make_pair(ii(hd,ad),ii(hk,ak)))==V.end()) {
		S.insert(make_pair(ii(hd,ad),ii(hk,ak)));
	}
}

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	cin>>t;
	for(int c=1;c<=t;++c) {
		cin>>hd>>ad>>hk>>ak>>b>>d;
		set<iii> S;
		S.insert(make_pair(ii(hd,ad),ii(hk,ak)));
		set<iii> V;
		bool found = false;
		for(int p=1;p<=100000 && !found;++p) {
			set<iii> new_S;
			for(set<iii>::iterator it=S.begin();it!=S.end();++it) {
				V.insert(*it);

				int new_hd = it->first.first;
				int new_ad = it->first.second;
				int new_hk = it->second.first;
				int new_ak = it->second.second;
				bool res = attack(new_hd, new_ad, new_hk, new_ak, new_S,V);
								buff(new_hd, new_ad, new_hk, new_ak, new_S,V);
				cure(new_hd, new_ad, new_hk, new_ak, new_S,V);
				debuff(new_hd, new_ad, new_hk, new_ak, new_S,V);
				if(res) {
					cout<<"Case #"<<c<<": "<<p<<endl;
					found = true;
					break;
				}


			}
			S = new_S;
		}
		if(!found) {
			cout<<"Case #"<<c<<": IMPOSSIBLE"<<endl;
		}
	}

	return 0;
}