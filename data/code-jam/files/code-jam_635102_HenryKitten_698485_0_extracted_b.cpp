#include <iostream>
#include <cstring>
#include <cstdio>

using namespace std;

bool Miss[11][1024];
int p;
int np;
int m[1024];
int nsel;

bool isValid(int idx){
	int cur = idx;
	int missed = 0;
	for(int i=0;i<p;i++){
		cur >>= 1;
		if(Miss[i][cur] == 0) missed++;
	}

	int i_f,cur_f;
	cur = idx;
	if(missed > m[idx]){
		for(int i=0;i<p;i++){
			cur >>= 1;
			if(Miss[i][cur] == 0){
				i_f = i;
				cur_f = cur;
			}
		}	
	}else{
		return true;
	}

	Miss[i_f][cur_f] = 1;
	nsel++;
	return false;
}

int main(){
	int ncas;
	cin >> ncas;
	for(int cas = 1; cas <= ncas; cas++){
		nsel = 0;
		scanf("%d",&p);
		np = 1<<p;
		for(int i=0;i<np;i++){
			scanf("%d",&m[i]);
		}
		for(int i=0;i<np-1;i++){
			int tmp;
			scanf("%d",&tmp);
//			printf("%d\n",tmp);
		}
		memset(Miss,0,sizeof(Miss));
		for(int i=0;i<np;i++){
		//	printf("%d\n",i);
			if(!isValid(i)){
				i=-1;
			}
		}

		printf("Case #%d: %d\n",cas,nsel);
	}
	return 0;
}
