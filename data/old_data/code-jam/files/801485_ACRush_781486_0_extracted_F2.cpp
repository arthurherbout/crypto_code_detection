#include <vector>
#include <list>
#include <map>
#include <set>
#include <deque>
#include <queue>
#include <stack>
#include <bitset>
#include <algorithm>
#include <functional>
#include <numeric>
#include <utility>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <cctype>
#include <string>
#include <cstring>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <string.h>

using namespace std;


int c[]={1,1,0,1,2,0,2,2,6,1,3,1,2,3,15,3,3,24,1,4,2,2,4,18,3,4,32,4,4,24,1,5,3,2,5,18,3,5,36,4,5,44,5,5,48,1,6,4,2,6,18,3,6,40,4,6,44,5,6,76,6,6,72,1,7,5,2,7,18,3,7,44,4,7,36,5,7,88,6,7,116,7,7,96,1,8,6,2,8,18,3,8,48,4,8,40,5,8,80,6,8,112,7,8,144,8,8,96,1,9,7,2,9,18,3,9,52,4,9,36,5,9,88,6,9,116,7,9,168,8,9,168,1,10,8,2,10,18,3,10,56,4,10,36,5,10,100,6,10,128,7,10,168,
	8,10,188,
	9,9,144,
	9,10,204,
	10,10,192};

int n,m;
int R[20][20];

int main(int argc,char **args)
{
	printf("Hello World!\n");
//	freopen("F.in","r",stdin);freopen("F.out","w",stdout);
//	freopen("F-small-attempt0.in","r",stdin);freopen("F-small-attempt0.out","w",stdout);
//	freopen("F-small-attempt1.in","r",stdin);freopen("F-small-attempt1.out","w",stdout);
	freopen("F-small-attempt10.in","r",stdin);freopen("F-small-attempt10.out","w",stdout);
//	freopen("F-large.in","r",stdin);freopen("F-large.out","w",stdout);
	memset(R,255,sizeof(R));
	for (int p=0;p<sizeof(c)/sizeof(int);p+=3)
		R[c[p]][c[p+1]]=R[c[p+1]][c[p]]=c[p+2];
/*
	for (int i=1;i<=10;i++)
		for (int j=1;j<=i;j++)
			printf("%d %d %d\n",i,j,R[i][j]);
	return 0;
*/
	int testcase;
	scanf("%d",&testcase);
	for (int caseId=1;caseId<=testcase;caseId++)
	{
		printf("Case #%d:",caseId);
		scanf("%d%d",&n,&m);
		printf(" %d\n",R[n][m]);
	}
	return 0;
}
