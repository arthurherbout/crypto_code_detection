#include<iostream>
using namespace std;
int main()
{
	int money,R,k,N,test;
	cin>>test;
	int i,j,op=0;
	while(test--)
	{
				 money=0;
				 cin>>R>>k>>N;
				 int grp[N],cost[N],ends[N];
				 for(i=0;i<N;i++)
				 {
								 cin>>grp[i];
				 }
                 for(i=0;i<N;i++)
				 {
								 cost[i]=grp[i];
								 int temp=grp[i],j=(i+1)%N;
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
				 int k=0;
				 while(R--)
				 {
						   money+=cost[k];
						   k=ends[k];
		   		 }
		   		 cout<<"Case #"<<++op<<": "<<money<<endl;
 	}
return 0;
}
