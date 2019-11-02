#include<stdio.h>
#include<vector>
using namespace std;
vector<int> v;

int a[] = {0,0,0,2, 3, 5, 8, 10, 13, 21, 24, 26, 34, 55, 63, 65, 68, 89, 144, 165, 168, 170, 178, 233, 377, 432, 440, 442, 445, 466, 610, 987, 1131, 1152, 1155, 1157, 1165, 1220, 1597, 2584, 2961, 3016, 3024, 3026, 3029, 3050, 3194, 4181, 6765, 7752, 7896, 7917, 7920, 7922};
int n;
void solve()
{
	int c = 0;


	printf("%d\n", a[n]);
}

int main()
{
#ifndef ONLINE_JUDGE
	//freopen("data.txt","r",stdin);
	freopen("C-small-attempt3.in","r",stdin);
	freopen("C-small-attempt3.out","w",stdout);
#endif

	int cases;
	scanf("%d\n", &cases);
	for(int i = 1; i <= cases; i++)
	{
		printf("Case #%d: ", i);
		scanf("%d", &n);
		solve();
	}


	return 0;

}
