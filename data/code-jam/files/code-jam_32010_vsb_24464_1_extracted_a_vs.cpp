#pragma comment(linker, "/STACK:134217728")

#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <cstring>

#include <vector>
#include <string>
#include <set>
#include <map>
#include <queue>
#include <sstream>
#include <iostream>
#include <algorithm>

using namespace std;

#define sqr(a) ((a)*(a))
#define det2(a,b,c,d) ((a)*(d) - (b)*(c))

#define FOR(i,a,b) for(i=(a); i<(b); ++i)

int main()
{
    int i, j;
    int TST, cas;
	int res;
    scanf( "%d", &TST );
    for ( cas = 1 ; cas <= TST ; cas++ )
    {
	int Ax1, Ax2;
	int Ay1, Ay2;
	int Bx1, Bx2;
	int By1, By2;
	int Cx1, Cx2;
	int Cy1, Cy2;
	scanf("%d%d%d%d%d%d", &Ax1, &Ay1, &Bx1, &By1, &Cx1, &Cy1);
	scanf("%d%d%d%d%d%d", &Ax2, &Ay2, &Bx2, &By2, &Cx2, &Cy2);

        
	int A1 = (Ax1 - Ax2) - (Cx1 - Cx2);
	int B1 = (Bx1 - Bx2) - (Cx1 - Cx2);
	int C1 = (Cx1 - Cx2);

	int A2 = (Ay1 - Ay2) - (Cy1 - Cy2);
	int B2 = (By1 - By2) - (Cy1 - Cy2);
	int C2 = (Cy1 - Cy2);

//	printf("1: %d %d %d\n", A1, B1, C1);
//	printf("2: %d %d %d\n", A2, B2, C2);

	int D = A1 * B2 - A2 * B1;
	int Da = -C1 * B2 + C2 * B1;
	int Db = -A1 * C2 + A2 * C1;

//	printf("D=%d Da=%d Db=%d\n", D, Da, Db);

        printf( "Case #%d: ", cas);

	double a, b;
        if(D == 0)
	{
		if(Da==0 && Db==0)
		{
			a=b=0;
		}
		else
		{
			a=b=-1;
		}
	}
	else
	{
		a = double(Da) / D;
	        b = double(Db) / D;
	}

	double eps = 1e-8;
	if(a!=-1/* && -eps <= a && a <= 1+eps && -eps <= b && b <= 1+eps*/)
	{
	        double x = a * Ax1 + b * Bx1 + (1-a-b) * Cx1;
	        double y = a * Ay1 + b * By1 + (1-a-b) * Cy1;
		printf("%.10lf %.10lf\n", x, y);

//	        double xx = a * Ax2 + b * Bx2 + (1-a-b) * Cx2;
//	        double yy = a * Ay2 + b * By2 + (1-a-b) * Cy2;
//		if(xx!=x || yy!=y)  printf("!!!%.10lf %.10lf\n", xx-x, yy-y);
	}
	else
		printf("No Solution\n");



    }

    
    return 0;
}

