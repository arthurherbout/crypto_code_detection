
#include <iostream>
#include <vector>
#include <fstream>
using namespace std;
vector < vector <long long> > box;
vector < vector <long long> > toy;

long long mark[110][110];


long long get_ans(int x,int y)
{

	if(x==box.size()|| y==toy.size())
		return 0;
	if(box[x][1] == toy[y][1])
	{
		long long min = box[x][0];
		if(min > toy[y][0])
		{
			min = toy[y][0];
			box[x][0] -= min;
			long long max = get_ans(x,y+1)+min;
			box[x][0]+=min;
			return max;
		}
		else
		{
			toy[y][0]-=min;
			long long max = get_ans(x+1,y)+min;
			toy[y][0]+=min;
			return max;
		}
	}
	long long max1 = get_ans(x,y+1);
	long long max2 = get_ans(x+1,y);
	if(max1>max2)
	{
		mark[x][y] = max1;
		return max1;
	}
	mark[x][y]=max2;
	return max2;
}

int main()
{
	int c;
	int i,j;
	int m,n;
	long long x,y;
	ofstream out("ans.out");
	cin>>c;
	for(i=1;i<=c;i++)
	{
		cin>>n>>m;
		box.clear();
		toy.clear();
		for(j=0;j<n;j++)
		{
			vector <long long> temp;
			cin>>x>>y;
			temp.push_back(x);
			temp.push_back(y);
			box.push_back(temp);
		}
		for(j=0;j<m;j++)
		{
			vector <long long> temp;
			cin>>x>>y;
			temp.push_back(x);
			temp.push_back(y);
			toy.push_back(temp);
		}

		out<<"Case #"<<i<<": "<<get_ans(0,0)<<endl;
	}
	return 1;
}