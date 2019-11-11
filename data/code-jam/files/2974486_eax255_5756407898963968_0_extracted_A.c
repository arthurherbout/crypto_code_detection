#include <stdio.h>
/*
0123
4567
89ab
cdef
choose card
get the row
reorder(easy-make sure every card that ws on known row is on different row this time)
get the row
*/
int main(){
	int T,i,r,a[2][4],v,j;
	scanf("%d",&T);
	for(i=1;i<=T;i++){
		printf("Case #%d: ",i);
		scanf("%d",&r);
		for(j=1;j<=4;j++){
			if(r==j)scanf("%d %d %d %d",a[0]+0,a[0]+1,a[0]+2,a[0]+3);
			else scanf("%d %d %d %d",&v,&v,&v,&v);
		}
		scanf("%d",&r);
		for(j=1;j<=4;j++){
			if(r==j)scanf("%d %d %d %d",a[1]+0,a[1]+1,a[1]+2,a[1]+3);
			else scanf("%d %d %d %d",&v,&v,&v,&v);
		}
		r=-2;//answer not found
		for(j=0;j<4;j++){
			for(v=0;v<4;v++){
				if(a[0][j]==a[1][v]){
					if(r==-2){r=a[0][j];break;}//each element unique
					else{r=-1;goto end;}//no need to compare more
				}
			}
		}
end:		if(r==-1)puts("Bad magician!");//too many things
//no answer,also same as -1 as this can only happen if there could be multiple answers
		else if(r==-2)puts("Volunteer cheated!");
		else printf("%d\n",r);//and the answer
	}
}


