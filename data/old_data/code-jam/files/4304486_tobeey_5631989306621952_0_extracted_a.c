#include<stdio.h>
#include<string.h>
int main(void){
	int test,count=0,i,fh,sh,len;
	scanf("%d",&test);
	while(test--){
		count++;
		char a[16],ans[50];
		
		for(i=0;i<50;i++)
			ans[i]='8';
		scanf("%s",a);
		len=strlen(a);
		ans[25]=a[0];
		for(i=1,fh=25,sh=26;i<len;i++){
			if(ans[fh]<=a[i]){
				fh--;
				ans[fh]=a[i];

			}
			else{
				ans[sh]=a[i];
				sh++;
			}
		}
 		printf("Case #%d:",count);	
		for(i=0;i<50;i++)
			if(ans[i]!='8')
				printf("%c",ans[i]);
		printf("\n");
	}
	return 0;
}
	
