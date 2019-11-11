#include <stdio.h>
int sum(int *, int s, int e);
void insertionsort(int *, char *, int , int);
int main(void){
	int test,z=0;
	scanf("%d",&test);	
	while(test--){
		z++;
		int n,i;
		int a[26],ans,count;
		char b[26]={'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
		scanf("%d",&n);
		for(i=0;i<n;i++)
			scanf("%d",&a[i]);
		printf("Case #%d: ",z);
		ans=sum(a,0,n);
		while(ans){
			insertionsort(a,b,0,n);
			if(a[n-1]==a[n-2]){
				if(sum(a,0,n-2)==0){
					a[n-1]--;
					a[n-2]--;
					printf("%c%c ",b[n-1],b[n-2]);
				}
				else{
					count=0;
						for(i=0;i<n-2 && count<2;i++){
							if(a[i]-2>0 && count<2){
								a[i]-=2;
								printf("%c%c",b[i],b[i]);
								count+=2;
							}	
							else if(a[i]>0 && count<2){
								a[i]--;	
								printf("%c",b[i]);
							}						
						}
						printf(" ");
				}
			}
			else{
				a[n-1]--;
				printf("%c ",b[n-1]);
			}
			ans=sum(a,0,n);
		}
		printf("\n");
	}
	return 0;
}
int sum(int a[],int s,int e){
	int i,sum=0;
	for(i=s;i<e;i++)
		sum+=a[i];
	return sum;
}
void insertionsort(int a[],char b[], int start, int end){
	int i,temp,j;
	char chartemp;
	for(i=start;i<end;i++){
		chartemp=b[i];
		temp=a[i];
		j=i-1;
		while((temp<a[j])&&(j>=0)){
			b[j+1]=b[j];
			a[j+1]=a[j];
          		j=j-1;
      		}
		b[j+1]=chartemp;
      		a[j+1]=temp;
 	}
}

