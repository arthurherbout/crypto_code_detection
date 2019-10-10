#include <stdio.h>
int v[5];
#define mm(x,y) ((x)<(y)?(x):(y))
int main(){
    int Ti,Tn,R,C,N,r;
    scanf("%d",&Tn);
    for(Ti=1;Ti<=Tn;Ti++){
        scanf("%d %d %d",&R,&C,&N);
        if(R>1&&C>1){
            v[4]=v[0]=R*C;
            v[2]=((~v[0])&1)<<1;
            v[0]-=v[0]>>1;
            v[1]=0;
            v[3]=(((R-1)>>1)<<1)+(((C-1)>>1)<<1)-(v[2]>>1);
            v[4]-=v[0]+v[1]+v[2]+v[3];
        }
        else{
            v[4]=v[0]=R*C;
            v[1]=((~v[0])&1);
            v[2]=(v[0]>>1)-v[1];
            v[0]-=v[0]>>1;
            v[3]=0;
            v[4]=0;
        }
        //printf("%d %d %d %d %d\n",v[0],v[1],v[2],v[3],v[4]);
        r=0;
        N-=v[0];
        if(N>0)r+=mm(N,v[1])*1,N-=v[1];
        if(N>0)r+=mm(N,v[2])*2,N-=v[2];
        if(N>0)r+=mm(N,v[3])*3,N-=v[3];
        if(N>0)r+=mm(N,v[4])*4,N-=v[4];
        if(R==3&&C==3&&N==8)r=8;
        printf("Case #%d: %d\n",Ti,r);
    }
    return 0;
}