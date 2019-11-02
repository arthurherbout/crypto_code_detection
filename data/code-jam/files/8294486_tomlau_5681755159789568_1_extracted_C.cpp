#include <bits/stdc++.h>
#include <queue>
using namespace std;

long long T, N, Q, D[128][128], E[128], S[128], U, V;
long long cd[128];
bool vis[128];

double d2[128][128];

int main() {
	cin >> T;
	for (int t=1; t<=T; t++) {
		cin >> N >> Q;
		for (int i=1; i<=N; i++)
			cin >> E[i] >> S[i];
		for (int i=1; i<=N; i++)
			for (int j=1; j<=N; j++)
				cin >> D[i][j];

		for (int i = 1; i <= N; i++) {
			for (int j = 1; j <= N; j++) {
				cd[j] = 2000000000;
				vis[j] = false;
				d2[i][j] = 1E50;
			}
			d2[i][i] = 0;
			cd[i] = 0;
			int next = i;

			/*
			for (int a = 1; a <= N; a++)
				for (int j = 1; j <= N; j++) if (cd[j] < 2000000000)
					for (int k = 1; k <= N; k++) if (D[j][k] != -1) {
						long long dist = cd[j] + D[j][k];
						if (dist < E[i] && dist < cd[k] && dist >= 0) {
							cd[k] = dist;
							double new_reach = d2[i][j] + ((double)dist) / S[i];
							if (new_reach < d2[i][k])
								d2[i][k] = new_reach;
						}
					}
				*/

			priority_queue<pair<int, int>> Q;
			
			while (next != -1) {
				int cur = next;
				vis[cur] = true;
				next = -1;
				for (int j=1; j<=N; j++)
					if (j != cur && !vis[j] && D[cur][j] > 0) {
						long long dist = cd[cur] + D[cur][j];
						if (dist <= E[i]) {
							if (dist < cd[j])
								cd[j] = dist;
							double new_reach = ((double)dist) / S[i];
							if (new_reach < d2[i][j])
								d2[i][j] = new_reach;
							Q.push(make_pair(-dist, j));
						}
					}
				if (Q.empty()) break;
				next = Q.top().second;
				Q.pop();
			}
			
		}

		cout << "Case #" << t << ": ";

		for (int q=0; q<Q; q++) {
			cin >> U >> V;

			double cd2[128];
			for (int j = 1; j <= N; j++) {
				cd2[j] = 1E50;
				vis[j] = false;
			}

			cd2[U] = 0;

			for (int i = 1; i <= N*20; i++)
				for (int j = 1; j <= N; j++)
					for (int k = 1; k <= N; k++) {
						if (cd2[j] + d2[j][k] < cd2[k])
							cd2[k] = cd2[j] + d2[j][k];
					}


			/*
			vis[U] = true;
			cd2[U] = 0;
			int next = U;

			while (next != -1) {
				double md = 1E50;
				int cur = next;
				next = -1;
				for (int j=1; j<=N; j++)
					if (j != cur) {
						double dist = cd2[cur] + d2[cur][j];
						if (dist < cd2[j]) {
							cd2[j] = dist;
							md = dist;
							next = j;
						}
					}
			}
			*/
			

			printf("%.9f ", cd2[V]);
		}
		cout << endl;
	}
}
