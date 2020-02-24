#include<cstdlib>
#include<iostream>
#include<fstream>
#include<cstring>
#include<math.h>
using namespace std;

int main()
{
	ifstream ifile;
	ofstream ofile;

	char cc[1000];
	int tc;
	char *x;
	int i,j,k,n,fl=0;
	int l,h,f;//f[10001];
	int prm[10001],valid[10001];
	int prmi;
	int no=6,rt;
	long double prod=1;
	ifile.open("in");
	ofile.open("out",ios::out);
	
	ifile.getline(cc,1000,'\n');			

	tc=atoi(cc);
	
	prm[0]=2;
	prm[1]=3;
	prm[2]=5;prmi=2;
	for(k=0;k<11001;k++,no++)
	{fl=0;rt=sqrt(no);
		for(i=0;prm[i]<=rt && i<=prmi;i++)
		{
			if(no%prm[i]==0){fl=1;break;}
		}
		if(fl==0){prm[++prmi]=no;}
	}
	for(k=0;k<=prmi;k++)
		cout<<prm[k]<<"\n";
		//6 2 10000 9929 9941 9967 9931 9973 9949
	for(k=0;k<10001;k++)
		valid[k]=0;
	
	for(k=0;k<tc;k++)
	{prod=1;
		cout<<"\n************case"<<k+1<<"*****************\n\n";
		ifile.getline(cc,1000,'\n');
		x = strtok(cc," ");
		n=atoi(x);
		x = strtok(NULL," ");
		l=atoi(x);
		x = strtok(NULL," ");
		h=atoi(x);
		cout<<n<<" "<<l<<" "<<h<<"\n";
		ifile.getline(cc,1000,'\n');
		for(i=0;i<n;i++)
		{
			if(i==0)x = strtok(cc," ");
			else x = strtok(NULL," ");
			f=atoi(x);
			for(j=0;prm[j]<=f;j++)
			{
				if(valid[j]==0)
				{
					if(f%prm[j]==0)valid[j]=1;
				}
			}
		}

		for(i=0;i<10001;i++)
		{
			if(valid[i]==1)prod=prod*prm[i];
			if(prod>h)break;
		}
		cout<<"product-"<<prod<<"\n";
		if(prod>=l && prod<=h)
			ofile<<"Case #"<<k+1<<": "<<prod<<"\n";
		else	ofile<<"Case #"<<k+1<<": NO\n";
		
		for(i=0;i<10001;i++)
			valid[i]=0;

	}
	
	
	ofile.close();
	ifile.close();

	return 0;
}
