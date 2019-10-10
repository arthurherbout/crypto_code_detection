# include <stdio.h>
# include <string.h>


char name[1000010] ;
long long dp[1000010], cc[1000010], ans[1000010] ;
long long len, n ;


long long vol(char ch)
{
	char s[] = "aeiou" ;
	long long i ;
	for (i = 0 ; i < 5 ; i++)
		if (ch == s[i]) return 1 ;
	return 0 ;
}


long long gao()
{
	long long i ;
	if (cc[0] == 0) ans[0] = 0, dp[0] = 0 ;
	else{
		if (n > 1) ans[0] = 0, dp[0] = 0 ;
		else ans[0] = 1LL, dp[0] = 1LL ;
	}
	for (i = 1 ; i < len ; i++)
	{
		ans[i] = ans[i-1] + dp[i-1] ;
		dp[i] = dp[i-1] ;
		if (cc[i] == n) ans[i]+= i-n+2LL-dp[i-1], dp[i] += i-n+2LL-dp[i-1] ;
		if (cc[i] > n) ans[i]++, dp[i]++ ;
	}
	long long rtn = 0 ;
	for (i = 0 ; i < len ; i++)
		rtn+=dp[i] ;
	return rtn ;
	//return ans[len-1] ;
}


int main ()
{
	long long T, nCase = 1 ;
	long long aa, i ;
	long long tmp ;
	freopen ("A-large.in", "r", stdin) ;
	freopen ("A-large.txt", "w", stdout) ;
	scanf ("%I64d", &T) ;
	while (T--)
	{
		scanf ("%s %I64d", name, &n) ;
		len = strlen(name) ;
		tmp = 0 ;
		for (i = 0 ; i < len ; i++)
		{
			if (vol(name[i])) tmp = cc[i] = 0 ;
			else tmp = cc[i] = tmp+1 ;
		}
		aa = gao() ;
		printf ("Case #%I64d: %I64d\n", nCase++, aa) ;
	}
	return 0 ;
}
