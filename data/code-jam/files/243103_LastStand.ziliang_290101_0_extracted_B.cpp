#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <ctime>
#include <cstring>
using namespace std;

const int MOD = 10009;

int ans[30];
string formular;
int K;
int n;

inline int cal(int cnt[])
{
	int ret = 0;
	int now = 1;
	formular = formular;
	for(unsigned i=0;i<=formular.size();i++)
	{
		if(formular[i]=='+' || i==formular.size())
		{
			ret += now;
			if(ret>=MOD)ret -= MOD;
			now = 1;
		}
		else 
		{
			now *= cnt[formular[i]-'a'];
			while(now>=MOD)now-=MOD;
		}
	}

	return ret;
}

void dfs(const char words[][64], int dep, int cnt[])
{
	//if(dep==K)return;

	for(int i=0;i<n;i++)
	{
		//strcpy(all+len, words[i]);
		
		for(int j=0;words[i][j];j++)
		{
			cnt[words[i][j]-'a']++;
		}

		ans[dep] += cal(cnt);
		if(ans[dep]>=MOD)ans[dep]-=MOD;
		
		if(dep+1<K)dfs(words, dep+1, cnt);

		for(unsigned j=0;words[i][j];j++)
		{
			cnt[words[i][j]-'a']--;
		}
	}
}
int main()
{
	freopen("smallin.txt","r",stdin);
	freopen("smallout.txt","w",stdout);

	int T;
	cin>>T;
	for(int t=1;t<=T;t++)
	{
		cin >> formular;
		cin>>K;
		
		cin>>n;
		char words[26][64];
		for(int i=0;i<n;i++)
		{
			cin>>words[i];
		}

		//
		cout<<"Case #" << t << ":";

		//char all[1024] = {0};
		memset(ans, 0, sizeof(ans));
		int cnt[30]={0};
		dfs(words, 0, cnt);

		for(int i=0;i<K;i++)
			cout<<" "<<ans[i];

		cout<<endl;
		//cout<<clock()<<endl;
	}
}

