#include <iostream>
#include <algorithm>
#include <string>
#include <queue>

using namespace std;

struct Time{
	int bg,ed;
	bool left;
	void init(const string& hr,const string& mn,bool l){
		bg = parse(hr);
		ed = parse(mn);
		left = l;
	}
	/*
	 *input : string
	 *output : abs time in that day
	 */
	int parse(const string& in){
		int idx = in.find(":");
		char hr[12],mn[12];
		strcpy(hr,in.c_str());
		hr[idx] = '\0';
		strcpy(mn,in.c_str()+idx+1);
		return atoi(hr)*60 + atoi(mn);
	}
	bool operator<(const Time& t) const{
		return bg < t.bg;
	}
};

int main(){
	int ncas,ti;
	Time tim[1024];
	for(cin >> ncas,ti=1;ti<=ncas;ti++){
		int t,nl,nr,nsl,nsr;
		string hr,mn;
		cin >> t >> nl >> nr;
		for(int i=0;i<nl;i++){
			cin >> hr >> mn;
			tim[i].init(hr,mn,1);
		}
		for(int i=0;i<nr;i++){
			cin >> hr >> mn;
			tim[i+nl].init(hr,mn,0);
		}

		sort(tim,tim+nl+nr);

		priority_queue<int,vector<int>,greater<int> > ldone,rdone;
		nsl = nsr = 0;
		for(int i=0;i<nl+nr;i++){
			if(tim[i].left){
				if(ldone.empty()){
				       	++nsl; 
				}else if(ldone.top() + t > tim[i].bg){
				       	++nsl;
				}else{
					ldone.pop();
				}
				rdone.push(tim[i].ed);
			}else{
				if(rdone.empty()){
				      	++nsr; 
				}else if(rdone.top() + t > tim[i].bg){
				       	++nsr;
				}else{
					rdone.pop();
				}
				ldone.push(tim[i].ed);
			}
		}
		printf("Case #%d: %d %d\n",ti,nsl,nsr);
	}
	return 0;
}
