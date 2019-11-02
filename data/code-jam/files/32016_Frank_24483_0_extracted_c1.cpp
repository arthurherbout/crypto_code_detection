#include <cstdio>
//#include<stdio.h>
#include<cmath>
using namespace std;

#define DEBUG 0
#define epsilon 1E-6
#define PI (4.*atan(1.))

char ans[][100]={"001", 
"005", 
"027",
"143",
"751",
"935",
"607",
"903", //7
"991",
"335", //9
"047", //10
"943", //11
"471",
"055",
"447",
"463", //15
"991",
"095",
"607",
"263",
"151", //20
"855",
"527",
"743",
"351",
"135", //25
"407",
"903",
"791",
"135",
"647"

};
int main()
{
	freopen("C-small-attempt0.in","r",stdin);

#if !DEBUG
	freopen ("C-small-attempt0.out","w",stdout);
#endif

	int cases, n;
	
	scanf("%d\n", &cases);

	for(int i=1;i<=cases;i++)
	{
		scanf("%d\n", &n);

		printf("Case #%d: %s\n", i, ans[n]);

	}
	//

#if !DEBUG
	fclose (stdout);
#endif

	return 0;
}