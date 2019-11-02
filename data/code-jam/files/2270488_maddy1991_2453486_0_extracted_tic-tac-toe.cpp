#include<iostream>
#include<cstdio>
#include<algorithm>
#include<cstdlib>
#include<cstring>
using namespace std;
int main()
{
		freopen("A-small-attempt0.in","r",stdin);
		freopen("output.txt","w",stdout);

	int t,j,i,k,p=0,dots,flag;
	char s[10][10];
	scanf("%d",&t);
	t=t+1;
	string dummy;
	getline(cin,dummy);
	while(t--)
	{
		p++; dots=0; flag=0;
		int x_r[5],o_r[5],t_r[5],x_c[5],o_c[5],t_c[5],x_d1=0,o_d1=0,t_d1=0,x_d2=0,o_d2=0,t_d2=0;
		for(i=1;i<=5;i++)
		{
			x_r[i]=0; o_r[i]=0; t_r[i]=0; x_c[i]=0; o_c[i]=0; t_c[i]=0; 
		}
		for(i=1;i<=4;i++)
		{
			for(j=1;j<=4;j++)
			{
				cin>>s[i][j];
				if(s[i][j]=='X')
				x_r[i]++;
				else if(s[i][j]=='O')
				o_r[i]++;
				else if(s[i][j]=='T')
				t_r[i]++;
				else if(s[i][j]=='.')
				dots++;
			}
		}
		/*for(i=1;i<=4;i++)
		{
			for(j=1;j<=4;j++)
			{
				cout<<s[i][j];
			}cout<<endl;
		}*/
	/*	cout<<"rows \n";
		for(i=1;i<=4;i++)
		{
			cout<<"x "<<x_r[i]<<"o "<<o_r[i]<<"t "<<t_r[i]<<endl;
		}
	*/
		for(j=1;j<=4;j++)
		{
			for(i=1;i<=4;i++)
			{
				if(s[i][j]=='X')
				x_c[j]++;
				else if(s[i][j]=='O')
				o_c[j]++;
				else if(s[i][j]=='T')
				t_c[j]++;
			}
		}
	/*	cout<<"colums \n";
		for(i=1;i<=4;i++)
		{
			cout<<"x "<<x_c[i]<<"o "<<o_c[i]<<"t "<<t_c[i]<<endl;
		}
		cout<<"dots "<<dots<<endl;
	*/
		for(i=1;i<=4;i++)
		{
			for(j=1;j<=4;j++)
			{
				if(i==j)
				{
					if(s[i][j]=='X')
					x_d1++;
					else if(s[i][j]=='O')
					o_d1++;
					else if(s[i][j]=='T')
					t_d1++;
				}
				if(i+j==5)
				{
					if(s[i][j]=='X')
					x_d2++;
					else if(s[i][j]=='O')
					o_d2++;
					else if(s[i][j]=='T')
					t_d2++;
				}
			}
		}
	/*	cout<<"diagonals ";
		cout<<"d1 "<<x_d1<<" "<<o_d1<<" "<<t_d1<<endl;
		cout<<"d2 "<<x_d2<<" "<<o_d2<<" "<<t_d2<<endl;
	*/	
		for(i=1;i<=4;i++)
		{
			if(x_r[i]==4 || (x_r[i]==3 && t_r[i]==1))
			{ printf("Case #%d: X won\n",p); flag=1; }
			else if(o_r[i]==4 || (o_r[i]==3 && t_r[i]==1))
			{ printf("Case #%d: O won\n",p); flag=1; }
			else if(x_c[i]==4 || (x_c[i]==3 && t_c[i]==1))
			{ printf("Case #%d: X won\n",p); flag=1; }
			else if(o_c[i]==4 || (o_c[i]==3 && t_c[i]==1))
			{ printf("Case #%d: O won\n",p); flag=1; }
		}
		if(x_d1==4 || (x_d1==3 && t_d1==1))
		{ printf("Case #%d: X won\n",p); flag=1; }
		else if(o_d1==4 || (o_d1==3 && t_d1==1))
		{ printf("Case #%d: O won\n",p); flag=1; }
		
		else if(x_d2==4 || (x_d2==3 && t_d2==1))
		{ printf("Case #%d: X won\n",p); flag=1; }
		else if(o_d2==4 || (o_d2==3 && t_d2==1))
		{ printf("Case #%d: O won\n",p); flag=1; }
		
		else if(dots>0 && flag==0)
		printf("Case #%d: Game has not completed\n",p);
		else if(dots==0 && flag==0) 
		printf("Case #%d: Draw\n",p);
					
			
	}
	
	return 0;
}
