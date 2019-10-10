#include <stdio.h>
#include <stdlib.h>


void rotate(char str[],int upto){
	int i;
	for (i = 0; i < upto; ++i)
	{
		str[i]=str[upto];
	}
}

int main(){
	int t;
	scanf("%d",&t);
	int iter;
	for (iter = 0; iter < t; ++iter)
	{
		char str[110];
		scanf("%s",str);
		int cnt=0;
		char c=str[0];
		int i=0;
		int last=0;
		while(c!='\0'){
			if (str[last]!=str[i])
			{
				rotate(str,i);
				cnt+=1;
				//printf("%s %d\n",str,cnt );
				                              
			}	
			last=i;
			i++;
			c=str[i];

		}
		if (str[0]=='-')
		{
			cnt+=1;
		}
		printf("Case #%d: %d\n",iter+1,cnt );
	}




	

}	