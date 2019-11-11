#include <cstdio>
int main()
{
	int a, b, d;
	scanf("%d", &d);
	for(int m=1; m<=d; m++)
	{
		scanf("%d %d", &a, &b);
		printf("Case #%d: ", m);
		int res=0;
		if(a<=1 && b>=1) res++;
		if(a<=4 && b>=4) res++;
		if(a<=9 && b>=9) res++;
		if(a<=121 && b>=121) res++;
		if(a<=484 && b>=484) res++;
		printf("%d\n", res);
	}	
	return 0;
}
