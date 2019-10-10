#include<stdio.h>
#include<iostream>
#include<cstring>
#include<string.h>

#define MAX 530
#define T(a,b) Tahta[a][b]
#define T2(a,b) Tahta2[a][b]
#define BAK(a) (a>='0' && a<='9') ? a-'0' : a-'A'+10
#define MIN(a,b) (a>b) ? b : a

int Tahta[MAX][MAX];
int Tahta2[MAX][MAX];
int N,M;

void Ekle(int a,int x,int y)
	{
	int mask=1;
	int t = (y+1)*4-1;
	for(int i=0; i<4; i++)
		{
		T(x,t) = (a & mask) ? 1 : 0;
		a>>=1;
		t--;
		}
	}

void Yazdir()
	{
	for(int j=0; j<M; j++,printf("\n"))
			for(int k=0; k<N; k++)
			printf("%3d",T(j,k));printf("\n");
	}
	
void Doldur()
	{
	int one,zero;
	for(int i=0; i<M; i++)
		{
		one=zero=1;
		T2(i,0)=1;
		if(T(i,0)==1)one++;
		else zero++;
		
		for(int j=1; j<N; j++)
			{
			if(T(i,j-1)==T(i,j))
				T2(i,j)=T2(i,j-1);
			else
				{
				if(T(i,j)==1)
					T2(i,j)=one++;
				else
					T2(i,j)=zero++;
				}
			}
		}
	}

void Kontrol()
	{
	int size = MIN(M,N)-1;
	int flag;
	int Dizi[MAX]={0};
	
	while(size>-1){
	for(int i=size; i<M; i++)
		for(int j=size; j<N; j++)
			{
			flag=0;
			for(int k=i-size; k<=i; k++)
				{
				for(int t=j-size; t<=j; t++)
					if( T(k,t)==-1 || (k+1<=i && T(k+1,t)==T(k,t)) || (t+1<=j && T(k,t+1)==T(k,t)))
						{
						flag=1;
						break;
						}
				if(flag)break;					
				}
			if(!flag)
				{
				Dizi[size]++;
				for(int k=i-size; k<=i; k++)
					for(int t=j-size; t<=j; t++)
						T(k,t)=-1;
			//	Yazdir();		
				}
			}
		size--;
		}
	
/*	for(int i=0; i<M; i++)
		for(int j=0; j<N; j++)
			if(T(i,j)!=-1)Dizi[0]++;
		*/
	
	size=0;
	for(int i=MIN(M,N)-1; i>=0; i--)
		if(Dizi[i])size++;
	printf("%d\n",size);
	for(int i=MIN(M,N)-1; i>=0; i--)
		if(Dizi[i])
			printf("%d %d\n",i+1,Dizi[i]);
	}

int main()
	{
	int Tcase;
	char a;
	scanf(" %d",&Tcase);
	for(int i=1; i<=Tcase; i++)
		{
		scanf(" %d %d",&M,&N);
		for(int j=0; j<MAX; j++)
			for(int k=0; k<MAX; k++)
				T(j,k)=0;
		
		
		for(int j=0; j<M; j++)
			for(int k=0; k<N/4; k++)
				{
				scanf(" %c",&a);
				Ekle(BAK(a),j,k);		
				}	//Yazdir();	
		printf("Case #%d: ",i);
		Kontrol();
		}
	return 0;
	}
