#include <stdio.h>
#include <iostream>
#include <stdint.h>
#include <string.h>
uint8_t bscp[512][8];
uint8_t bs[512][8];
uint64_t *bss=(uint64_t*)bs;
#define set(i,x) (bs[(x)][(i)>>3]|=1<<((i)&7))
#define get(i,x) (bs[(x)][(i)>>3]&(1<<((i)&7)))
char v[64];
int chk(int N){
	int i,j;
	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			if(bss[i+N]==bss[j])goto found;
		}
		return 0;
found:		continue;
	}
	return 1;
}
int main(void){
	int T,Ti;
	int N,L,i,j;
	uint64_t ans=0;
	scanf("%d\n",&T);
	for(Ti=1;Ti<=T;Ti++){
		memset(bs,0,512*8);
		scanf("%d %d",&N,&L);
		for(i=0;i<N;i++){
			scanf("%s",v);
			for(j=0;j<L;j++){
				if(v[j]=='0')continue;
				set(j,i);
			}
		}
		for(i=0;i<N;i++){
			scanf("%s",v);
			for(j=0;j<L;j++){
				if(v[j]=='0')continue;
				set(j,i+N);
			}
		}
		memcpy(bscp,bs,512*8);
		for(ans=0;ans<(1L<<L);ans++){
			memcpy(bs,bscp,512*8);
			for(j=0;j<N;j++){
				bss[j]^=ans;
			}
			if(chk(N))goto answ;
		}
		printf("Case #%d: NOT POSSIBLE\n",Ti);
		continue;
answ:		j=0;
		while(ans>0)j+=ans&1,ans>>=1;
		printf("Case #%d: %d\n",Ti,j);
	}
	return 0;
}

