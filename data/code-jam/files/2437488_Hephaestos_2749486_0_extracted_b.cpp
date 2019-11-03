#include <iostream>
#include <vector>
#include <string>

using namespace std;


int main(void) {
	int T;
	cin>>T;
	for (int i=0;i<T;++i){
		string result;
		int x,y;
		cin>>x>>y;
		bool inv_x = x<0;
		bool inv_y = y<0;
		x = abs(x);
		y = abs(y);
		int j=0;
		bool done = false;
		int mx(0);
		int my(0);
		while (!done) {
			++j;
			if (max(x-mx,y-my)>=j) {
				if (x-mx >y-my) {
					mx += j;
					result +='E';
				} else {
					my +=j;
					result += 'N';
				}
			} else {
				for (int k=0;k<x-mx;++k) {
					result+="WE";
				}
				for (int k=0;k<y-my;++k) {
					result += "SN";
				}
				done = true;
			}
		}	
		if (inv_x) {
			for (char& c:result) {
				if (c=='E' || c=='W') {
					c = (c=='E'?'W':'E');
				} 
			}
		}		
		if (inv_y) {
			for (char& c:result) {
				if (c=='N' || c=='S') {
					c = (c=='N'?'S':'N');
				} 
			}
		}
		cout<< "Case #"<<i+1<<": "<<result<<'\n';
	}
	return 0;
}