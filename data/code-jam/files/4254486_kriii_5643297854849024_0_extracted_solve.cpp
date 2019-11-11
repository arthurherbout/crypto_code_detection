#include <stdio.h>
#include <algorithm>
using namespace std;

int N,D,A[5050],B[5050],C[5050];

int main()
{
	freopen ("input.txt","r",stdin);
	freopen ("output.txt","w",stdout);

	int Test; scanf ("%d",&Test); for (int Case=1;Case<=Test;Case++){
		printf ("Case #%d: ",Case);
		scanf ("%d %d",&N,&D);
		for (int i=0;i<N;i++) scanf ("%d",&A[i]); A[N] = A[0];
		bool good = 1;
		for (int i=2*D;i<N;i++) if (A[i] != A[i-2*D]) good = 0;
		if (!good){
			puts("CHEATERS!");
			continue;
		}

		int ans = 0;
		while (D >= 1){
			for (int i=0;i<=D;i++) B[i] = A[(i+D)%(2*D)] - A[i];
			bool good = 1;
			for (int i=0;i<D;i++){
				int j = i + 1;
				if ((B[i] - B[j]) % 2) good = 0;
				C[i] = (B[i] - B[j]) / 2;
				ans += abs(C[i]);
			}
			if (!good){
				ans = -1; break;
			}
			for (int i=0;i<D;i++){
				if (C[i] > 0) for (int j=0;j<D;j++) A[(i+D*2-j)%(2*D)] += C[i];
				else for (int j=0;j<D;j++) A[(i+D-j)%(2*D)] -= C[i];
			}
			D /= 2;
		}
		if (ans < 0) puts("CHEATERS!");
		else printf ("%d\n",ans);
	}

	return 0;
}