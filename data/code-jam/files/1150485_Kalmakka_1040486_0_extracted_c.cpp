#include <iostream>
#include <vector>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cmath>
using namespace std;

bool colorize(vector<int>&posts, vector<vector<int> > &rooms, int roomnum, int postnum, int colors) {
	if (roomnum == rooms.size()) return true;
	if (postnum == rooms[roomnum].size()) {
		vector<bool> seen(colors+1, false);
		int cc = 0;
		for (int i=0;i<postnum;i++) {
			if (seen[posts[rooms[roomnum][i]]] == false) {
				seen[posts[rooms[roomnum][i]]] = 1;
				cc++;
			}
		}
		if (cc != colors) return false;
		else return colorize(posts,rooms,roomnum+1,0, colors);
	}
	int post = rooms[roomnum][postnum];
	if (posts[post]) return colorize(posts,rooms,roomnum,postnum+1,colors);
	else {
		for (int i=1;i<=colors;i++) {
			posts[post] = i;
			bool ok = colorize(posts, rooms, roomnum, postnum+1, colors);
			if (ok) return true;
		}
		posts[post] = 0;
		return false;
	}
}

void print(vector<int> &v) {
	for (int i=0;i<v.size();i++)
		cout << v[i] << " ";
	cout << endl;
}
void tc(int tcn) {
	int n,m;
	cin>>n>>m;
	vector<int> start(m);
	vector<int> end(m);

	for (int i=0;i<m;i++){cin>>start[i]; start[i]--; }
	for (int i=0;i<m;i++){cin>>end[i]; end[i]--; }

	vector<vector<int> > rooms;
	vector<int> house;
	for (int i=0;i<n;i++)
		house.push_back(i);
	rooms.push_back(house);
	for (int i=0;i<m;i++) {
		for (int j=0;j<rooms.size();j++) {
			if ((find(rooms[j].begin(),rooms[j].end(),start[i]) != rooms[j].end()) &&
				(find(rooms[j].begin(),rooms[j].end(),end[i]) != rooms[j].end())) {
				vector<int> room1;
				vector<int> room2;
				int dist = (end[i] - start[i] + n) % n;
				for (int p=0;p<rooms[j].size();p++) {
					int pnum = rooms[j][p];
					if (pnum == start[i] || pnum==end[i]) {
						room1.push_back(pnum);
						room2.push_back(pnum);
					}
					else {
						int d2 = (pnum - start[i] + n) % n;
						if (d2 < dist)
							room1.push_back(pnum);
						else
							room2.push_back(pnum);
					}
				}
				rooms[j] = room1;
				rooms.push_back(room2);
				j = rooms.size();
			}
		}
	}
	int minSize = 3000;
	for (int i=0;i<rooms.size();i++)
		minSize = min(minSize, (int)rooms[i].size());
	for (int colors=minSize;colors>=1;colors--) {
		vector<int> posts(n, 0);
		if (colorize(posts, rooms, 0, 0, colors)) {
			cout << "Case #" << tcn << ": " << colors << endl;
			cout << posts[0];
			for (int i=1;i<posts.size(); i++)
				cout << " " << posts[i];
			cout << endl;
			return;
		}
	}
}
int main() {
	int t;
	cin>>t;
	for (int i=1;i<=t;i++) tc(i);
}