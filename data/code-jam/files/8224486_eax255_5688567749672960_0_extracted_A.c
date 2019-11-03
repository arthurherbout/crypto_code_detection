#include <stdio.h>
int k[10101010];
int rev(int N){
    int v=0;
    while(N){
        v*=10;
        v+=N%10;
        N/=10;
    }
    return v;
}
int main(){
    int Tn,N,Ti,r,v,nv,l,i;
    scanf("%d",&Tn);
    for(i=1;i<=1000000;i++)k[i]=i;
    for(i=1;i<=1000000;i++){
        if(k[i+1]>k[i]+1)k[i+1]=k[i]+1;
        if(k[rev(i)]>k[i]+1)k[rev(i)]=k[i]+1;
    }
    for(Ti=1;Ti<=Tn;Ti++){
        scanf("%d",&N);
        if(N>1000000)return 1;
        r=k[N];
        printf("Case #%d: %d\n",Ti,r);
    }
    return 0;
}
