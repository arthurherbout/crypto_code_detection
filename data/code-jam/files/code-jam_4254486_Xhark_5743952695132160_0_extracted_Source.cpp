#include <algorithm>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <set>
using namespace std;

#include <math.h>
#include <stdio.h>
int P;
int E[10000], F[10000];
vector<pair<int, int> > EF;
int Qe[10000], Qf[10000], head;
bool poscheck(int v) {
	vector<pair<int, int> > cEF = EF, nEF;
	int tail = 0;
	head = 0;
	bool found = false;
	for (int i = 0; i < (int)cEF.size(); i++) {
		if (cEF[i].first == v) {
			found = true;
		}
	}
	if (!found) return false;
	if (v < 0) {
		for (int i = 0; i < (int)cEF.size(); i++) {
			int e = cEF[i].first;
			int f = cEF[i].second;
			if (tail < head) {
				if (Qe[tail] == e) {
					f -= Qf[tail];
					tail++;
				}
			}

			if (f > 0) {
				Qe[head] = e - v;
				Qf[head] = f;
				head++;
				nEF.push_back(make_pair(e - v, f));
			}
			else if (f < 0) {
				return false;
			}
		}
	}
	else if (v == 0) {
		for (int i = 0; i < (int)cEF.size(); i++) {
			int e = cEF[i].first;
			int f = cEF[i].second;
			if (f % 2 != 0) return false;
			nEF.push_back(make_pair(e, f / 2));
		}
	}
	else {
		for (int i = (int)cEF.size() - 1; i >= 0; i--) {
			int e = cEF[i].first;
			int f = cEF[i].second;
			if (tail < head) {
				if (Qe[tail] == e) {
					f -= Qf[tail];
					tail++;
				}
			}

			if (f > 0) {
				Qe[head] = e - v;
				Qf[head] = f;
				head++;
				nEF.push_back(make_pair(e - v, f));
			}
			else if (f < 0) {
				return false;
			}
		}
		reverse(nEF.begin(), nEF.end());
	}
	if (tail != head) return false;
	EF = nEF;
	return true;
}
int main(){
	freopen("D-small-attempt0.in", "r", stdin);
	freopen("D-small-attempt0.out", "w", stdout);
	int T;
	scanf("%d", &T);
	for (int cs = 1; cs <= T; cs++) {
		scanf("%d", &P);
		for (int i = 0; i < P; i++) {
			scanf("%d", &E[i]);
		}
		for (int i = 0; i < P; i++) {
			scanf("%d", &F[i]);
		}
		EF.clear();
		for (int i = 0; i < P; i++) {
			EF.push_back(make_pair(E[i], F[i]));
		}
		sort(EF.begin(), EF.end());
		printf("Case #%d: ", cs);
		for (int i = 0; i < P; i++) {
			if (poscheck(E[i]))
			{
				printf("%d ", E[i]);
				i--;
				continue;
			}
		}
		if (EF.size() > 1 || EF[0].second > 1) fprintf(stderr, "FAIL");
		printf("\n");
	}

	return 0;
}