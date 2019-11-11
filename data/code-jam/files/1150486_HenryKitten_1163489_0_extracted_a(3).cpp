#include <stdio.h>
#include <algorithm>
#define MAXN 1000
//#define DEBUG 1

using namespace std;

class Segment {
	public:
		bool operator<(const Segment& rhs) const {
			return w < rhs.w;
		}
		int w, l;
};
int main()
{
	int last_pos;
	int S, R, N, X;
	int space, begin, end, speed;
	double remain, total, duration, t;
	Segment seg[MAXN+1];
	int n_case;

	scanf("%d", &n_case);
	for (int i_case=1; i_case<=n_case; ++i_case) {
		scanf("%d%d%d%lf%d", &X, &S, &R, &t, &N);
		last_pos = space = 0;
		for (int i=0; i<N; ++i) {
			scanf("%d%d%d", &begin, &end, &speed);
			space += begin - last_pos;
			last_pos = end;
			seg[i].w = speed;
			seg[i].l = end - begin;
		}
		seg[N].w = 0;
		seg[N].l = space + X - last_pos;
		++N;
		sort(seg, seg+N);
#ifdef DEBUG
		for (int i=0; i<N; ++i) {
			printf("speed: %d length: %d\n", seg[i].w, seg[i].l);
		}
#endif
		total = 0;
		for (int i=0; i<N; ++i) {
			duration = (double)seg[i].l/(R+seg[i].w);
			if (duration <= t) {
#ifdef DEBUG
				printf("%lf\n", duration);
#endif
				t -= duration;
				total += duration;
			} else {
				//printf("duration :%.10lf, t: %.10lf\n", duration, t);
				duration = t;
				t = 0;
				total += duration;
				remain = seg[i].l - duration*(R+seg[i].w);
				total += remain/(S+seg[i].w);
			}
		}
		printf("Case #%d: %.9lf\n", i_case, total);
	}
	
	return 0;
}
