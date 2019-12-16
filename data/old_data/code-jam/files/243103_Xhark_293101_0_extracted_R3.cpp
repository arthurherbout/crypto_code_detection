#include <stdio.h>
long long Pel[ 20000000 ], pph;
int main(){
	freopen("input.txt","r",stdin);
	FILE *out = fopen("output.txt","w");
	long long L, R;
	int T;
	scanf("%d",&T);
	long long i, j, pel, ti, tti, A, B, sol, ph, temp, Rock[2], tph;
	A = 1; B = 10;
	while(B <= 10000000){
		int tx;
		for(i=A;i<B;i++){
			ti = i / 10;
			pel = 1;
			while(ti > 0){
				pel *= 10;
				ti = ti / 10;
			}
			pel *= i;
			ti = i / 10;
			tti = 0;
			while(ti > 0){
				tti = tti * 10 + ti % 10;
				ti = ti / 10;
			}
			pel += tti;
			Pel[ pph ++ ] = pel;
		}
		if(i < B) break;
		for(i=A;i<B;i++){
			ti = i;
			pel = 1;
			while(ti > 0){
				pel *= 10;
				ti = ti / 10;
			}
			pel *= i;
			ti = i;
			tti = 0;
			while(ti > 0){
				tti = tti * 10 + ti % 10;
				ti = ti / 10;
			}
			pel += tti;
			
			Pel[ pph ++ ] = pel;
		}
		if(i < B) break;
		A = A * 10; B = B * 10;
	}
	while(T>0){
		scanf("%lld %lld",&L,&R);
		int i;
		int X, Y;
		X = -1; Y = -2;
		tph = 0;
		for(i=0;i<pph;i++){
			if(L <= Pel[i] && Pel[i] <= R){
				if(X == -1) X = i;
				Y = i;
				tph ++;
			}
			if(Pel[i] > R) break;
		}
		Rock[0] = Rock[1] = 0;
		int t;
		t = 0;
		sol = 0;
		ph = 0;

		for(i=X;i<=Y;i++){
			pel = Pel[i];
			ph ++;
			temp = pel - L - 1;
			if(ph == 1) temp ++;
			if(temp >= 0){
				temp %= 1000000007;
				sol += ((( Rock[t] + ph/2 + (tph-ph)/2 )%1000000007) * temp)%1000000007;
				sol %= 1000000007;
				sol += (temp * (temp+1)) / 2;
				sol %= 1000000007;
				Rock[t] += temp;
				Rock[t] %= 1000000007;
			}
			L = pel;
			t = 1-t;
		}
		temp = R - L;
		if(ph == 0) temp ++;
		if(temp >= 0){
			int i;
			for(i=0;i<temp;i++){
				Rock[t] ++;
				sol += Rock[t] + ph/2 + (tph-ph)/2;
				sol %= 1000000007;
			}
		}
		for(i=ph-1;i>=1;i--){
			sol += (i+1)/2;
			sol %= 1000000007;
		}
		static int ttt = 1;
		printf("%d\n",ttt);
		fprintf(out, "Case #%d: %lld\n", ttt++, sol);
		T--;
	}
	fclose(out);
	return 0;
}