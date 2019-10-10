#include <cstdio>

int main()
{
	int d;
	float C, F, X;
	scanf("%d", &d);
	for(int i=1; i<=d; i++)
	{
		scanf("%f %f %f", &C, &F, &X);
		//printf("%f %f %f", C, F, X);
		float t=0.0, k=0.0;
		while(C + X*(2.0+k*F)/(2.0+k*F+F) <= X)
		{
			t += C/(2.0+k*F);
			k += 1.0;
		}
		t += X/(2.0+k*F);
		printf("Case #%d: %.7f", i, t);
		
		if(i<d)
			puts("");
	}
	return 0;
}
