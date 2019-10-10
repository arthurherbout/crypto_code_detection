#include <stdio.h>
#include <string>
#include <iostream>
#include <algorithm>
using namespace std;

char str[10000] ;
int length , ans = 0 ; 

__int64 Abs(__int64 a )
{
	if(a < 0)
		return -a ;
	return a; 
}
__int64 judge(__int64 a)
{
	if(a < 0)
		return -1 ;
	return 1 ;
}

void DFS(int pos, __int64 sum , __int64 pre)
{
	if(pos == length)
	{
		int temp = sum + pre;	
		 if(temp % 2 == 0 || temp % 3 == 0 || temp % 5 == 0 || temp % 7 == 0)
			ans ++ ;
		return ;
	}
	DFS(pos + 1 , sum + pre, str[pos]-'0') ;
	DFS(pos + 1 , sum - pre, str[pos]-'0') ;
	DFS(pos + 1 , sum , judge(pre)*(Abs(pre * 10) + str[pos]-'0')) ;
	return ;
}

int main()
{
	freopen("B-small-attempt1.in","r",stdin);
	freopen("B.out","w",stdout);
	int text , Case ;
	while ( 1 == scanf("%d",&text))
	{
		for (Case = 0 ; Case < text ; Case ++)
		{ 
			scanf("%s",str) ;
			ans  = 0 ;
			length = strlen(str) ;
			DFS( 1 , 0 , str[0]-'0') ;
			printf("Case #%d: %d\n",Case+1,ans);
		}
	}
	return 0 ; 
}