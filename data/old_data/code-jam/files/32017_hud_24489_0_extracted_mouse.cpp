#include "iostream"
#include <stdlib.h>
#include <math.h>
#include <vector>

using namespace std;


int main()
{
	int T;
	scanf("%d\n",&T);
	for (int c = 0; c < T; c++)
	{
		int K,n;
		scanf("%d %d",&K,&n);
		
		vector<int> data;
		data.push_back(K);
		int pos = 0;
		for(int i = K-1; i > 0; i--)
		{
//			for(int j = 0; j<data.size(); j++)
//			{
//				printf("%d ",data[j]);
//			}
//			printf("\n pos= %d \n",pos);
			pos = (pos-(i%data.size())+data.size())%data.size();
			data.insert(data.begin()+pos,i);
		}
		int i;
		for(i = 0; i<K; i++)
		{
			if (data[i] == 1)
				break;
		}
		rotate(data.begin(), data.begin()+i, data.end());

		printf("Case #%d: ",c+1);
		for (i = 0; i < n; i++)
		{
			int q;
			scanf(" %d",&q);
			printf("%d ",data[q-1]);
		}
		printf("\n");
		scanf("\n");
		

	}
	return 0;
}
