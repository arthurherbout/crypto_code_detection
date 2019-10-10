#include <stdio.h>
#include <string.h>
int main(){
    int Tn,Ti,R,C,W,r;
    scanf("%d",&Tn);
    for(Ti=1;Ti<=Tn;++Ti){
        scanf("%d %d %d",&R,&C,&W);
        if(W==C)r=W+R-1;
        else if(W==1)r=R*C;
        else if(W>C/2)r=R+W;
        else if(W==C/2&&(C&1))r=R*2+W;
        else if(W==C/2)r=R*2-1+W;
        else if(W<C/2&&C%W!=0)r=R*(C/W)+W;
        else if(W<C/2)r=R*(C/W)+W-1;
        printf("Case #%d: %d\n", Ti,r);
    }
    return 0;
}