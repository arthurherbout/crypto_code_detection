#include <stdio.h>
int main(){
    int Ti,T;
    int X,R,C,r;
    scanf("%d",&T);
    for(Ti=1;Ti<=T;Ti++){
        scanf("%d %d %d",&X,&R,&C);
        if(X==1)r=1;
        if(X==2)r=!(R&C&1);
        if(X==3)r=(R%3==0||C%3==0)&&(C>1&&R>1);
        if(X==4){
            if(R<C)R^=C,C^=R,R^=C;
            if(R<4)r=0;
            else if(C<3)r=0;
            else r=1;
        }
        printf("Case #%d: %s\n",Ti,r?"GABRIEL":"RICHARD");
    }
    return 0;
}

