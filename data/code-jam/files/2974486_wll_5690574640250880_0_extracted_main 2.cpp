#include<iostream>
#include<cstdio>
#include<cmath>
using namespace std;
char mat[55][55];

int min(int a,int b)
{
	return a<b?a:b;
}
int max(int a,int b)
{
	return a<b?b:a;
}
int main()
{
    freopen("1.in","r",stdin);
	freopen("1.out","w",stdout);
    int t;
	scanf("%d",&t);
	int i;
	for(i=1;i<=t;i++)
	{
		printf("Case #%d:\n",i);
		int j,k;
		for(j=0;j<55;j++)
			for(k=0;k<55;k++)
				mat[j][k]='*';

         int r,c,m;
		 scanf("%d %d %d",&r,&c,&m);
		 int space=r*c-m;
		 int x=-1,y=-1;
		 if(space%r==0&&space/r>1)
		 {
		    x=r;
			y=space/r;
		 }else if(space%c==0&&space/c>1)
		 {
		    y=c;
			x=space/c;
		 }
		 else 
		 {
		    int lim=sqrt(space);
			for(int a=2;a<=lim;a++)
			{
			   if(space%a==0&&a<=min(r,c)&&space/a<=max(r,c))
			   {
			      if(r<c)
				  {
				     x=a;
					 y=space/a;
				  }else
				  {
				     x=space/a;
					 y=a;
				  }
				  break;
			   }
			   
			}
			if(space==1)
			   {
			     x=1;
				 y=1;
			   }
		 }

		 if(x+y<0)
		 {
		     int lim=sqrt(space);
			 int cc,vv;
			 if(space/r>1&&(space-r*(space/r))>1)
			 {
			      for(cc=0;cc<r;cc++)
					  for(vv=0;vv<space/r;vv++)
						  if(cc+vv==0)
							  mat[cc][vv]='c';
						  else
							  mat[cc][vv]='.';
                  for(cc=0;cc<space-r*(space/r);cc++)
					  mat[cc][space/r]='.';
				  for(cc=0;cc<r;cc++)
				  {
					for(vv=0;vv<c;vv++)
						printf("%c",mat[cc][vv]);
					printf("\n");
				  }

			 }else if(space/c>1&&(space-c*(space/c))>1)
			 {
			    for(cc=0;cc<space/c;cc++)
					for(vv=0;vv<c;vv++)
					    if(cc+vv==0)
							mat[cc][vv]='c';
						else
							mat[cc][vv]='.';
				for(cc=0;cc<(space-c*(space/c));cc++)
					mat[space/c][cc]='.';

				for(cc=0;cc<r;cc++)
				  {
					for(vv=0;vv<c;vv++)
						printf("%c",mat[cc][vv]);
					printf("\n");
				  }
			 }else if((space-lim*lim)==1||((space-lim*lim)>lim&&(space-lim*lim)/2==1)||lim>min(r,c))
			           printf("Impossible\n");
			      else
				  {
					  if((space-lim*lim)>lim)
					  {
						for(cc=0;cc<lim;cc++)
						  for(vv=0;vv<lim;vv++)
							  if(cc+vv==0)
								  mat[cc][vv]='c';
							  else
								  mat[cc][vv]='.';
						for(cc=0;cc<(space-lim*lim)/2;cc++)
							  mat[lim][cc]='.';
						int cont;
						if((space-lim*lim)%2==1)
                            cont=1;
						else
							cont=0;
						for(cc=0;cc<(space-lim*lim)/2+cont;cc++)
							  mat[cc][lim]='.';
						for(cc=0;cc<r;cc++)
							{
								for(vv=0;vv<c;vv++)
								printf("%c",mat[cc][vv]);
								printf("\n");
							}
					  }
						else
						{
						   for(cc=0;cc<lim;cc++)
						      for(vv=0;vv<lim;vv++)
							     if(cc+vv==0)
								   mat[cc][vv]='c';
							     else
								   mat[cc][vv]='.';
						   for(cc=0;cc<(space-lim*lim);cc++)
							  mat[lim][cc]='.';
						   for(cc=0;cc<r;cc++)
							{
								for(vv=0;vv<c;vv++)
								printf("%c",mat[cc][vv]);
								printf("\n");
							}
						}
				  }
		 
		 }
		 else
		 {
		    for(int z=0;z<x;z++)
				for(int v=0;v<y;v++)
				{
				   if(z==0&&v==0)
					   mat[z][v]='c';
				   else
					   mat[z][v]='.';
				}
			for(z=0;z<r;z++)
			{
				for(int v=0;v<c;v++)
					printf("%c",mat[z][v]);
		        printf("\n");
			}
		 }

        
		 
	}
    return 0;
}