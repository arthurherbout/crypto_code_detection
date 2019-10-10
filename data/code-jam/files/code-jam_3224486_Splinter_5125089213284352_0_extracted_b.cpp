#include <bits/stdc++.h>
using namespace std;

char ch[201];
int n , m;
char buf[200];
int fail[200];


int p[200];
bool vis[200];
int lst[101] , num;
int pos[101];

int main()
{
	int test;scanf("%d" , &test);
	for(int tt = 1 ; tt <= test ; tt++)
	{
		printf("Case #%d:" , tt);
		scanf("%d" , &n);
		for(int i = 1 ; i <= n ; i++)
			scanf("%d" , &p[i]);
		scanf("%s" , ch + 1);

		scanf("%d" , &m);
		for(int j = 1 ; j <= m ; j++)
		{
			scanf("%s" , buf + 1);
			int len = strlen(buf + 1);
			fail[1] = 0;
			for(int i = 2 ; i <= len ; i++)
			{
				int k = fail[i - 1];
				while(k && buf[k + 1] != buf[i])k = fail[k];
				if(buf[k + 1] == buf[i])fail[i] = k + 1;
				else fail[i] = 0;
			}

			int fm = 0 , fz = 0;
			for(int randtime = 1 ; randtime <= 20000 ; randtime++)
			{
				num = n;
				for(int i = 0 ; i < n ; i++)
					lst[i] = i + 1 , vis[i + 1] = false , pos[i + 1] = i;
				vis[0] = true;

				int nowlen = 0;bool app = false;
				for(int i = 1 ; i <= n ; i++)
				{
					int newnode = lst[rand() % num];
					while(!vis[p[newnode]])newnode = p[newnode];
					vis[newnode] = true;

					while(nowlen && buf[nowlen + 1] != ch[newnode])
						nowlen = fail[nowlen];
					if(buf[nowlen + 1] == ch[newnode])
						nowlen++;
					else nowlen = 0;
					if(nowlen == len)
					{
						app = true;
						break;
					}

					swap(lst[pos[newnode]] , lst[num - 1]);
					pos[lst[pos[newnode]]] = pos[newnode];
					pos[newnode] = -1;
					num--;
				}
				if(app)fz++;
				fm++;
			}
			printf(" %.7f" , double(fz) / fm);
		}
		printf("\n");
	}
}