/*
Coder : Kushagra Gour
chinchang457@gmail.com
23 may 2010

complexity : --
*/
#include<iostream>
#include<fstream>
#include<stack>
#include<queue>
#include<vector>
#include<set>
#include<string>
#include<stdio.h>
#include<math.h>
#include<time.h>
using namespace std;

#define DEBUG 1
#define FOR(i,a,b) for(int i=(int)(a) ; i < (int)(b);++i)
#define REP(i,n) FOR(i,0,n)

int main(){
	FILE *f; 
	FILE *pf;
	f=fopen("B-small-attempt1.in","r+");
	pf=fopen("B-small.out","w+");
	
	int t; fscanf(f,"%d",&t);	
	int l,p,c;	
	
	REP(i,t){		
		fscanf(f,"%d %d %d",&l,&p,&c);
		int ans=0;
		int tl,tu;
		if(l*c<p){
			do{
				tl=l*c; tu=(p%2?(p+1)/c:p/c);
				if(tu>tl){
					ans+=2;
					l=tl; p=tu;
				}
				else break;
			}while(1);
		}	
		cout<<ans<<endl;

		fprintf(pf,"Case #%d: %d\n",i+1,ans);
	}
}