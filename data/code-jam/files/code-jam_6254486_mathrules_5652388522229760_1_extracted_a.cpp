#include<iostream>
#include<conio.h>
#include<string.h>
#include<algorithm>
using namespace std;


int allcheck(int a[])
{
	int i=0,sum=0;
	for(i=0;i<10;i++)
	{
		sum+=a[i];
	}
	if(sum==10)
		return 1;
	else 
		return 0;
}


int main() {
    long long int t,o=1,num=0;
    cin>>t;
    while(t--)
    //while(1)
    {
     int all[10]={0};
     long long int x=0,i=1,sum=0,curnum=0,dupnum=0,flag=0;
     cin>>num;
     //num++;
     dupnum=num;
    while(1)
    {	
    	if(num==0)
    	{
    		flag=0;
    		break;
    	}
     curnum=num;
     while(num!=0)
     { 
     	x=num%10;
     	all[x]=1;
     	num=num/10;
     }
     
     	
     	flag=allcheck(all);
     	if(flag==1)
     		{
     			break;
     		}
     	i++;
     	num=dupnum*i;	

        
	}	
     	if(flag==1)
            cout<<"Case #"<<o<<": "<<curnum<<endl;
        else 
            cout<<"Case #"<<o<<": "<<"INSOMNIA"<<endl;
o++;
    }
     
    getch();
return 0;         
}
