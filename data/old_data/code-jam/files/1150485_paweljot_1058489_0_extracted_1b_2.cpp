#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <climits>
#include <cstring>
#include <deque>
#include <cmath>

using namespace std;

int main() {

	int T;
	cin >> T;
	deque<int> pointsQ;
	map<int,int> points;
	for (int t = 1; t <= T; t++) {

//		int points[200];
//		memset(points,0,sizeof(points));
		pointsQ.clear();

		int C, D;
		cin >> C;
		cin >> D;

		int minP = INT_MAX;
		int maxP = INT_MIN;
		int pointsNum = 0;
		for (int i = 0; i < C; i++) {
			int P, V;
			cin >> P;
			cin >> V;
			if (P > maxP)
				maxP = P;
			if (P < minP)
				minP = P;

			pointsQ.push_back(P);
			points[P] = V;
			pointsNum +=V;

		}

		int spread = (pointsNum - 1) * D;
		double center = (double)(minP+maxP) / (double)2;
		double halfspread = (double) spread / (double)2;
		double endpointRight = center + halfspread;
		double endpointLeft = center - halfspread;

		double maxDistance = 0;
		while (!pointsQ.empty()) {
			int p = pointsQ.front();
			if (points[p] == 1) pointsQ.pop_front();
			points[p]--;

			double distance = fabs((double)p-endpointLeft);
			if (distance> maxDistance) maxDistance = distance;
			endpointLeft += (double)D;
		}


		cout << "Case #" << t << ": " << maxDistance << endl;
	}

	return 0;
}
