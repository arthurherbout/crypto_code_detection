#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <algorithm>

using namespace std;

ifstream fin ("B.in");
ofstream fout ("B.out");

int s2i(string s){
    s[s.find(':')]=' ';
    istringstream iss(s);
    int h,m;
    iss>>h>>m;
    return h*60+m;
}

int main (){
	int n;
	fin>>n; 
	for (int a=1; a<=n; a++){
		int tt,na,nb;
		fin>>tt>>na>>nb;
		vector <int> sa(na),la(nb),sb(nb),lb(na);
		string s,t;
		for (int b=0; b<na; b++) {
			fin>>s>>t;
			sa[b]=s2i(s);
			lb[b]=s2i(t)+tt;
		}
		for (int b=0; b<nb; b++) {
			fin>>s>>t;
			sb[b]=s2i(s);
			la[b]=s2i(t)+tt;
		}
		sort(sa.begin(),sa.end());
		sort(sb.begin(),sb.end());
		sort(la.begin(),la.end());
		sort(lb.begin(),lb.end());
		int ta=0,tb=0;
		for (int b=0; b<na; b++){
			int c=0;
			for (; c<nb; c++){
				if (la[c]<=sa[b]){
					la[c]=1000000000;
					break;
				}
			}
			if (c==nb) ta++;
		}
		for (int b=0; b<nb; b++){
			int c=0;
			for (; c<na; c++){
				if (lb[c]<=sb[b]){
					lb[c]=1000000000;
					break;
				}
			}
			if (c==na) tb++;
		}
		fout<<"Case #"<<a<<": "<<ta<<' '<<tb<<endl;			
	}
}
