#include <iostream>
#include <string>
#include <math.h>

#include <stdlib.h>
#include <stdio.h>
#include <list>

#define REP(i,n) for(int i=0;i<(n);i++)
#define FOR(v,p,k) for(int v=p;v<=k;v++)

using namespace std;

int get_int()
{
	int val;
	cin>>val;
	return val;
}



bool opened[200];
bool could_open[200];
list<int> ch_key_op[401];
list<int> key_ch_co[200];
int key_need[200];
int n_can_open[401];
list<int> keys_to_test;
list<int> result;
int n_opened;
int n_has[401];
bool can_reach_key[401];

void opens(int chest)
{
	opened[chest]=true;
	n_opened++;
	int key=key_need[chest];
	n_can_open[key]--;
	n_has[key]--;
	for(list<int>::iterator key=key_ch_co[chest].begin();key!=key_ch_co[chest].end();key++)
	{
		keys_to_test.push_front(*key);
		n_has[*key]++;
	}
	could_open[chest]=false;
	result.push_back(chest);
}

bool test_reach_fr_key(int key,int key_dest);

bool test_reach_fr_ch(int chest,int key_dest)
{
	list<int> &l_k_obtain=key_ch_co[chest];
	for(list<int>::iterator key=l_k_obtain.begin();key!=l_k_obtain.end();key++)
	{
		if(can_reach_key[*key]==false)//pour l'instant, depuis la chest
		{
			can_reach_key[*key]=true;
			if(test_reach_fr_key(*key,key_dest))return true;
		}
	}
	return false;
}
bool test_reach_fr_key(int key,int key_dest)
{
	if(key==key_dest)return true;
	list<int> &l_ch_open=ch_key_op[key];
	for(list<int>::iterator chest=l_ch_open.begin();chest!=l_ch_open.end();chest++)
	{
		if(test_reach_fr_ch(*chest,key_dest))return true;
	}
	return false;
}

bool reach_key(int chest)
{
	for(int key=0;key<401;key++)
	{
		can_reach_key[key]=false;
	}
	return test_reach_fr_ch(chest,key_need[chest]);
}

void test_prem()
{
	int key=keys_to_test.front();
	keys_to_test.pop_front();
	if(n_can_open[key]<=n_has[key])
	{
		for(list<int>::iterator chest=ch_key_op[key].begin();chest!=ch_key_op[key].end();chest++)
		{
			if(!opened[*chest])
			{
				could_open[*chest]=true;
			}
		}
		return;
	}
	list<int> &l_ch_open=ch_key_op[key];
	for(list<int>::iterator chest=l_ch_open.begin();chest!=l_ch_open.end();chest++)
	{
		if(reach_key(*chest))could_open[*chest]=true;
	}
}

int main()
{
	int t,k,n;
	cin>>t;
	FOR(c,1,t)
	{
		n_opened=0;
		result.clear();
		REP(chest,200)
		{
			opened[chest]=false;
			could_open[chest]=false;
			key_ch_co[chest].clear();
		}
		REP(key,401)
		{
			ch_key_op[key].clear();
			n_can_open[key]=0;
		}
		cin>>k;
		cin>>n;
		keys_to_test.clear();
		FOR(i,1,k)
		{
			int key=get_int();
			keys_to_test.push_front(key);
			n_has[key]++;
		}
		REP(i,n)
		{
			int key=get_int();
			ch_key_op[key].push_front(i);
			key_need[i]=key;
			n_can_open[key]++;
			int m=get_int();
			REP(j,m)
			{
				key_ch_co[i].push_front(get_int());
			}
		}
		bool res=false;
		while(true)
		{
			while(!keys_to_test.empty())
			{
				test_prem();
			}
			int min_good=-1;
			REP(chest,n)
			{
				if(min_good==-1 && could_open[chest])
				{
					min_good=chest;
					break;
				}
			}
			if(min_good==-1)
			{
				res=false;
				break;
			}
			opens(min_good);
			if(n_opened==n)
			{
				res=true;
				break;
			}
		}
		printf("Case #%d: ",c);
		if(!res)printf("IMPOSSIBLE\n");
		else
		{
			for(list<int>::iterator key=result.begin();key!=result.end();key++)
			{
				cout<<*key+1<<" ";
			}
			cout<<endl;
		}
	}
}
