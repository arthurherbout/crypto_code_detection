#include <stdio.h>
char c[20000];
//char cc[20000];
char q[256][256];
char qr[256][256];
char qv[8]={'1','2','i','I','j','J','k','K'};
char inv[256];

int asd(int L,int X){
    int i;
    char vv='1';
    for(i=0;i<X*L;i++){
        vv=q[vv][c[i%L]];
        if(vv=='i')goto jck;
    }
    return 0;
jck:vv='1',++i;
    for(;i<X*L;i++){
        vv=q[vv][c[i%L]];
        if(vv=='j')goto kck;
    }
    return 0;
kck:vv='1',++i;
    for(;i<X*L;i++)vv=q[vv][c[i%L]];
    return vv=='k';
}

int main(){
	int Ti,T;
    int L,X;
	int r,i,j;
	scanf("%d",&T);
    q['1']['1']=q['2']['2']='1';
    q['i']['I']=q['I']['i']='1';
    q['j']['J']=q['J']['j']='1';
    q['k']['K']=q['K']['k']='1';
    
    q['1']['2']=q['2']['1']='2';
    q['I']['I']=q['i']['i']='2';
    q['J']['J']=q['j']['j']='2';
    q['K']['K']=q['k']['k']='2';
    
    q['i']['1']=q['1']['i']='i';
    q['i']['2']=q['2']['i']='I';
    q['I']['1']=q['1']['I']='I';
    q['I']['2']=q['2']['I']='i';
    
    q['j']['1']=q['1']['j']='j';
    q['j']['2']=q['2']['j']='J';
    q['J']['1']=q['1']['J']='J';
    q['J']['2']=q['2']['J']='j';
    
    q['k']['1']=q['1']['k']='k';
    q['k']['2']=q['2']['k']='K';
    q['K']['1']=q['1']['K']='K';
    q['K']['2']=q['2']['K']='k';
    
    q['i']['j']=q['j']['I']='k';
    q['i']['J']=q['J']['I']='K';
    q['I']['j']=q['j']['i']='K';
    q['I']['J']=q['J']['i']='k';
    
    q['j']['k']=q['K']['j']='i';
    q['j']['K']=q['k']['j']='I';
    q['J']['k']=q['K']['J']='I';
    q['J']['K']=q['k']['J']='i';
    
    q['k']['i']=q['I']['k']='j';
    q['k']['I']=q['i']['k']='J';
    q['K']['i']=q['I']['K']='J';
    q['K']['I']=q['i']['K']='j';
    
    for(i=0;i<8;i++)for(j=0;j<8;j++)qr[qv[i]][q[qv[i]][qv[j]]]=qv[j];
    
    inv['1']='2',inv['2']='1';
    inv['i']='I',inv['I']='i';
    inv['j']='J',inv['J']='j';
    inv['k']='K',inv['K']='k';
    
	for(Ti=1;Ti<=T;Ti++){
        scanf("%d %d",&L,&X);
        scanf("%s",c);
        r=1;
        if(L==1)r=0;
        else if(L==2&&X==1)r=0;
        else r=asd(L,X);
		printf("Case #%d: %s\n",Ti,r?"YES":"NO");
	}
}
