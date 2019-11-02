#include<stdio.h>
char s[1001];
int main()
{ 
    FILE *p,*o;
    p=fopen("A-large.in","r");
    o=fopen("out.out","w");
	int t,max,i,j,frnd,sum;
	fscanf(p,"%d",&t);
    for(i=1;i<=t;i++)
	{
		sum=frnd=0;
		fscanf(p,"%d",&max);
	//	s=new char[max+2];
		fscanf(p,"%s",s);
		for(j=1;j<=max;j++)
		{
			   	sum+=s[j-1]-48;
				if(sum<j&&(s[j]!='0'))
				{
					frnd+=j-sum;
					sum+=frnd;
				}
		}
	 // cout<<"Case #"<<i<<":"<<" "<<frnd<<"\n";
	 fprintf(o,"Case #%d: %d\n",i,frnd);
	}
	return 0;
}
