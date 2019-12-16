#include<stdio.h>
void main(){
	int not,i;
	scanf("%d",&not);
	int r,x,c;
	for(i=0;i<not;i++){
		scanf("%d",&x);
		scanf("%d",&r);
		scanf("%d",&c);
		
		if ((r*c)%x == 0)
			printf("case #%d: GABRIEL\n",i);
		else printf("case #%d: RICHARD\n",i);
	}
}
