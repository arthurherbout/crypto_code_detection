#include<iostream>
#include<string>
#include<fstream>
using namespace std;
#define maxn 200
char let[26]={'y','h','e','s','o','c','v','x','d','u','i','g','l','b','k','r','z','t','n','w','j','p','f','m','a','q'};
int main()
{
	ifstream in("A-small-attempt5.in");
	ofstream out("A-small-attempt5.out");
	string que,ans,tmp;
	int j,i,n;
	in>>n;
	getline(in,que); 
	for (j=1;j<=n;j++)
	{
		int l;
		ans="";	
		getline(in,que); 
		l=que.length();
		for (i=0;i<l;i++)
			if (que[i]==' ')
				ans=ans+' ';
			else
				ans=ans+let[(que[i]-'a')];
		out<<"Case #"<<j<<": "<<ans<<endl; 		
	}
	in.close();
	out.close();
}