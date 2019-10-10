// a.cpp : Defines the entry point for the console application.
//

#include <iostream>
using namespace std;
#include <string>
#include <set>
#include <string.h>

void main()
{
	/*freopen("input.txt", "r", stdin);
	freopen("code.txt", "w", stdout);
	char mas['z'+1];
	string s = "", t, m = "";
	do{
		cin>>t;
		s+=t;
	}while(t!="jv");
	do{
		cin>>t;
		m+=t;
	}while(t!="up");
	set<char> abc;
	for(char c = 'a'; c<='z'; c++)
		abc.insert(c);
	mas['q']='z';	
	for(int i=0; i<s.length(); i++)
		mas[s[i]]=m[i];
	for(char i='a'; i<'z'; i++)
		abc.erase(mas[i]);
	mas['z'] = *abc.begin();
	for(char i='a'; i<='z'; i++)
		printf("mas['%c']='%c';\n", i, mas[i]);
		//cout<<i<<": "<<mas[i]<<endl;
	*/
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
	char mas['z'+1];
mas['a']='y';
mas['b']='h';
mas['c']='e';
mas['d']='s';
mas['e']='o';
mas['f']='c';
mas['g']='v';
mas['h']='x';
mas['i']='d';
mas['j']='u';
mas['k']='i';
mas['l']='g';
mas['m']='l';
mas['n']='b';
mas['o']='k';
mas['p']='r';
mas['q']='z';
mas['r']='t';
mas['s']='n';
mas['t']='w';
mas['u']='j';
mas['v']='p';
mas['w']='f';
mas['x']='m';
mas['y']='a';
mas['z']='q';
	int T;
	cin>>T;
	char str[105];
	cin.getline(str, 105);
	for(int t=0; t<T; t++)
	{		
		cin.getline(str, 105);
		for(int i=0; i<strlen(str); i++)
			if(str[i]>='a' && str[i]<='z')
				str[i] = mas[str[i]];
		printf("Case #%d: %s\n", t+1, str);
	}
}

