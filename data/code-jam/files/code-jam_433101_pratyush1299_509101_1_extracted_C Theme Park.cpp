#include<cstdio>
using namespace std;
int main()
{
	long long money,R,k,N,test;
	scanf("%lld",&test);
	long long i,j,op=0;
	while(test--)
	{
				 money=0;
				 scanf("%lld%lld%lld",&R,&k,&N);
				 long long grp[N],cost[N],ends[N];
				 for(i=0;i<N;i++)
				 {
								 scanf("%lld",&grp[i]);
				 }
                 for(i=0;i<N;i++)
				 {
								 cost[i]=grp[i];
								 long long temp=grp[i],j=(i+1)%N;
								 if(j==i)
								 {
										 ends[i]=j;
                                         continue;
								 }
								 while((temp+grp[j])<=k)
								 {
														temp+=grp[j];
														j=(j+1)%N;
														if(j==i) break;
	  							 }
	  							 ends[i]=j;
	  							 cost[i]=temp;
				 }
//				 for(i=0;i<N;i++)
//				 cout<<cost[i]<<endl;
				 long long k=0;
				 while(R--)
				 {
						   money+=cost[k];
                  		   k=ends[k];
		   		 }
		   		 op++;
		   		 printf("Case #%lld: %lld\n",op,money);
 	}
return 0;
}
