#include <iostream>
#include <algorithm>
#include <map>
#include <queue>
using namespace std;
#define MAXX 15
#define POOL_SIZE 60000000
#define HASH 2147483647
#define PRIME1 97
#define PRIME2 101

const int dir[4][2]=
{
	0,1,0,-1,1,0,-1,0
};

int r,c;
int n;
bool board[MAXX][MAXX];
int head,tail;
pair<int,int> pair_pool[POOL_SIZE];

void state_sort(pair<int,int>* p)
{
	sort(p,p+n);
}

long long dboard[MAXX][MAXX];
long long d_cnt;

bool check_dangerous(pair<int,int>* p)
{
	if (n==1) return false;
	d_cnt++;
	for (int i=0;i<n;i++)
		dboard[p[i].first][p[i].second]=d_cnt;
	for (int i=0;i<n;i++)
	{
		int x=p[i].first;
		int y=p[i].second;
		bool flag=true;
		for (int j=0;j<4;j++)
			if (dboard[x+dir[j][0]][y+dir[j][1]]==d_cnt)
			{
//				cerr<<x<<' '<<y<<' '<<j<<endl;
				flag=false;
				break;
			}
		if (flag) return true;
//		cerr<<x<<' '<<y<<' '<<"danger"<<endl;
	}
	return false;
}

int calc_hash(pair<int,int>* p)
{
	long long ans=0;
	for (int i=0;i<n;i++)
	{
		ans*=PRIME1;
		ans+=p[i].first;
		ans*=PRIME2;
		ans+=p[i].second;
		ans%=HASH;
//		cerr<<p[i].first<<' '<<p[i].second<<endl;
	}
//	cerr<<ans<<endl;
	return ans;
}

int calc_forward(pair<int,int>* p,bool dang)
{
	int ans=0;
	for (int i=0;i<n;i++)
		board[p[i].first][p[i].second]=false;
	pair<int,int>* q=pair_pool+tail;
	for (int i=0;i<n;i++)
	{
		int x=p[i].first;
		int y=p[i].second;
		for (int j=0;j<4;j++)
			if (board[x+dir[j][0]][y+dir[j][1]] && board[x-dir[j][0]][y-dir[j][1]])
			{
				memcpy(q,p,sizeof(pair<int,int>)*n);
				q[i].first+=dir[j][0];
				q[i].second+=dir[j][1];
				state_sort(q);
//				for (int i=0;i<n;i++)
//					cerr<<q[i].first<<' '<<q[i].second<<endl;
//				cerr<<endl;
				if (dang && check_dangerous(q)) continue;
				q+=n;
				if (q==pair_pool+POOL_SIZE) q=pair_pool;
				ans++;
			}
	}
	for (int i=0;i<n;i++)
		board[p[i].first][p[i].second]=true;
	return ans;
}
				
int calc_backward(pair<int,int>* p,bool dang)
{
	for (int i=0;i<n;i++)
		board[p[i].first][p[i].second]=false;
	int ans=0;
	pair<int,int>* q=pair_pool+tail;
	for (int i=0;i<n;i++)
	{
		int x=p[i].first;
		int y=p[i].second;
		for (int j=0;j<4;j++)
			if (board[x+dir[j][0]][y+dir[j][1]] && board[x+dir[j][0]*2][y+dir[j][1]*2])
			{
				memcpy(q,p,sizeof(pair<int,int>)*n);
				q[i].first+=dir[j][0];
				q[i].second+=dir[j][1];
				state_sort(q);
//				for (int i=0;i<n;i++)
//					cerr<<q[i].first<<' '<<q[i].second<<endl;
//				cerr<<endl;
				if (dang && check_dangerous(q)) continue;
				q+=n;
				if (q==pair_pool+POOL_SIZE) q=pair_pool;
				ans++;
			}
	}
	for (int i=0;i<n;i++)
		board[p[i].first][p[i].second]=true;
	return ans;
}

map<int,int> hash;
queue<pair<pair<int,int>*,pair<bool,int> > > states;

void enqueue(int h,bool dang,int depth)
{
	hash[h]=depth;
	states.push(make_pair(pair_pool+tail,make_pair(dang,depth)));
	tail+=n;
	if (tail==POOL_SIZE) tail=0;
}

void dequeue(pair<int,int>*& now,bool& dang,int& depth)
{
	now=states.front().first;
	dang=states.front().second.first;
	depth=states.front().second.second;
	head+=n;
	if (head==POOL_SIZE) head=0;
	states.pop();
}

int ans;

int caseN;
int main()
{
	cin>>caseN;
	for (int caseI=1;caseI<=caseN;caseI++)
	{
		memset(board,false,sizeof(board));
		cin>>r>>c;
		head=tail=0;
		int temp1=0;
		int temp2=60;
		int wcnt=0;
		n=0;
		for (int i=1;i<=r;i++)
			for (int j=1;j<=c;j++)
			{
				char ch;
				cin>>ch;
				if (ch=='#')
					continue;
				else
				{
					board[i][j]=true;
					if (ch=='o' || ch=='w') pair_pool[temp1++]=make_pair(i,j);
					if (ch=='x' || ch=='w') pair_pool[temp2++]=make_pair(i,j);
					if (ch=='w') wcnt++;
				}
			}
		n=temp1;
//		cerr<<n<<' '<<wcnt<<endl;
		if (wcnt==n)
		{
			ans=0;
			goto output;
		}
		memcpy(pair_pool+n,pair_pool+60,sizeof(pair<int,int>)*n);
		state_sort(pair_pool+tail);
		enqueue(calc_hash(pair_pool+tail),false,0);
		state_sort(pair_pool+tail);
		enqueue(calc_hash(pair_pool+tail),false,-1);
		ans=-1;
		while (states.size() && ans==-1)
		{
			pair<int,int>* now;
			bool dangerous;
			int depth;
			int cnt;
			dequeue(now,dangerous,depth);
//			cerr<<"depth : "<<depth<<endl;
//			for (int i=0;i<n;i++)
//				cerr<<now[i].first<<' '<<now[i].second<<endl;
//			cerr<<endl;
			if (depth>=0)
				cnt=calc_forward(now,dangerous);
			else
				cnt=calc_backward(now,dangerous);
			int pt=tail;
			while (cnt--)
			{
//				cerr<<"pt "<<pt<<endl;
				int h=calc_hash(pair_pool+pt);
				bool d=check_dangerous(pair_pool+pt);
				map<int,int>::iterator it=hash.find(h);
				if (it==hash.end())
				{
					for (int j=0;j<n;j++)
						pair_pool[tail+j]=pair_pool[pt+j];
					enqueue(h,d,depth>=0 ? depth+1 : depth-1);
				}
				else
				{
					int h2=it->second;
					if (depth>=0 && h2<0)
					{
						ans=depth-h2;
						break;
					}
					if (depth<0 && h2>=0)
					{
						ans=h2-depth;
						break;
					}
				}
				pt+=n;
				if (pt==POOL_SIZE)
					pt=0;
			}
		}
output:		cout<<"Case #"<<caseI<<": "<<ans<<endl;
		hash.clear();
		while (states.size()) states.pop();
	}
	return 0;
}




