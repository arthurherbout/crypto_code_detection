#include<stdio.h>
#include<stdlib.h>
#include<algorithm>
#include<string.h>
#pragma warning(disable:4996)

int dp[20][3]; //0:rock,1:scissor,2:paper

int power(int n) {
	int num = 1;
	int i;
	for (i = 1; i <= n; i++) num *= 2;
	return num;
}

char * outp(int cur, int n, char state) {
	if (cur == n) {
		char *temp = (char *)malloc(sizeof(char) * 2);
		*temp = state;
		*(temp + 1) = '\0';
		return temp;
	}
	char *temp1=NULL, *temp2=NULL;
	char *ans = (char *)malloc(sizeof(char) *(power(cur) + 1));
	switch (state) {
	case 'R': 
		temp1 = outp(cur + 1, n, 'R');
		temp2 = outp(cur + 1, n, 'S'); break;
	case 'S' :
		temp1 = outp(cur + 1, n, 'P');
		temp2 = outp(cur + 1, n, 'S'); break;
	case 'P' :
		temp1 = outp(cur + 1, n, 'P');
		temp2 = outp(cur + 1, n, 'R'); break;
	}
	if (strcmp(temp1, temp2) == -1) ans = strcat(temp1, temp2);
	else ans = strcat(temp2, temp1);
	return ans;
}

int main() {
	freopen("A-small-attempt1.in", "r", stdin);
	freopen("A-small.out", "w", stdout);
	int m, n, i, j;
	int t, ti;
	int R, P, S;
	scanf("%d", &t);
	dp[0][0] = 1;
	dp[0][1] = 0;
	dp[0][2] = 0;
	for (i = 1; i <= 12; i++) {
		dp[i][0] = dp[i - 1][0] + dp[i - 1][2];
		dp[i][1] = dp[i - 1][1] + dp[i - 1][0];
		dp[i][2] = dp[i - 1][2] + dp[i - 1][1];
	}
	

	for (ti = 0; ti < t; ti++) {
		printf("Case #%d: ", ti + 1);
		scanf("%d", &n);
		scanf("%d %d %d", &R, &P, &S);
		if (R == dp[n][0] && S == dp[n][1] && P == dp[n][2])		printf("%s", outp(0, n, 'R'));
		else if (S == dp[n][0] && P == dp[n][1] && R == dp[n][2])	printf("%s", outp(0, n, 'S'));
		else if (P == dp[n][0] && R == dp[n][1] && S == dp[n][2])	printf("%s", outp(0, n, 'P'));
		else printf("IMPOSSIBLE");
		printf("\n");
	}

	return 0;

}