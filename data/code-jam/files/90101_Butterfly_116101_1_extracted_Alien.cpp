/*
Coder: Butterfly21
Task: Alien Language
Source: Google Code Jam 2009 Qualification Round Problem A
*/
#include<cstdio>
#include<cstring>
#include<algorithm>
struct dic{
	char C[20];
}S[5001];
bool operator<(dic A,dic B){return strcmp(A.C,B.C)<0;}
int L,D,N,cnt,Ca=1;
char P[1500];
void check(){
	cnt=0;
	for(int i=0,k;i<D;i++){
		k=0;
		for(int j=0;k<L;j++){
			int ill=0;
			if(P[j]=='('){
				j++;
			    while(1){
					//printf("%c %c\n",P[j],S[i].C[k]);
					if(P[j]==')'){
						ill=1;
						break;
					}else if(P[j]==S[i].C[k]){
						k++;
						while(P[j]!=')')j++;
						break;
					}
					j++;
				}
			}  //
			else if(P[j]!=S[i].C[k])ill=1;
			else k++;
			if(ill)break;
		}
		if(k==L){/*puts(S[i].C);*/cnt++;}
	}
}
int main(){
	freopen("A-large.in","r",stdin);
	freopen("A-large.out","w",stdout);
	scanf("%d%d%d",&L,&D,&N);
	for(int i=0;i<D;i++)
		scanf("%s",S[i].C);
	std::sort(S,S+D);
	getchar();
	for(int i=0;i<N;i++){
		gets(P);
		check();
		printf("Case #%d: %d\n",Ca++,cnt);
	}
	//scanf("  ");
}
