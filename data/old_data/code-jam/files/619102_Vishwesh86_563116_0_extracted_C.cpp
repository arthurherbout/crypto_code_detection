#include<iostream>
#include<fstream>
#include<vector>
#include<algorithm>
using namespace std;
class node
{
	public:
	int r,c;
	int sz;
	bool gone;
	bool val;
	bool visited;
	node()
	{
		sz=1;
		gone=false;
		val=false;//has to be init
		visited=false;
	}
	void init(int _r,int _c,int _sz,bool _gone,bool _val,bool _visited)
	{
		r=_r;
		c=_c;
		sz=_sz;
		gone=_gone;
		val=_val;
		visited=_visited;
	}
};
bool operator < (node a,node b)
{
	if(a.sz>b.sz)
		return true;
	if(a.sz<b.sz)
		return false;
	if(a.r<b.r)
		return true;
	if(a.r>b.r)
		return false;
	if(a.c<b.c)
		return true;
	//if(a.c>b.c)
		return false;
}
node rect[512][512];
int isvalid(int r,int c,int M,int N)
{
	if(!(r>=0 && r<M && c>=0 && c<N))
		return -1;
	if(rect[r][c].gone)
		return -1;
	if(rect[r][c].visited)
		return rect[r][c].sz;
	int d=isvalid(r+1,c,M,N);
	int b=isvalid(r,c+1,M,N);
	int dr=isvalid(r+1,c+1,M,N);
	if(d!=-1 && b!=-1 && dr!=-1)
	{
		if((rect[r][c].val == rect[r+1][c+1].val) && (rect[r+1][c].val==rect[r][c+1].val) && (rect[r+1][c].val != rect[r][c].val))
		{
			rect[r][c].sz=1+min(d,min(b,dr));
		}
	}
	rect[r][c].visited=true;
	return rect[r][c].sz;
}
int getint(char c)
{
	if(c>='0' && c<='9')
		return c-'0';
	return 10+(c-'A');
}
int main()
{
	ifstream fin("C.in");
	ofstream fout("C.out");
	int T,t;
	fin>>T;
	for(t=1;t<=T;++t)
	{
	//	cout<<"case:\n";
		fout<<"Case #"<<t<<": ";
		int M,N;
		fin>>M>>N;
		int i,j,k;
		char c;
		for(i=0;i<M;++i)
		{
			for(j=0;j<N/4;++j)
			{
				fin>>c;
				int mask=getint(c);
				rect[i][4*j].init(i,4*j,1,false,((mask&8)!=0),false);
				rect[i][4*j+1].init(i,4*j+1,1,false,((mask&4)!=0),false);
				rect[i][4*j+2].init(i,4*j+2,1,false,((mask&2)!=0),false);
				rect[i][4*j+3].init(i,4*j+3,1,false,((mask&1)!=0),false);
			}
		}
		vector<node> arr;
		for(i=0;i<M;++i)
		{
			for(j=0;j<N;++j)
			{
				int tmp=isvalid(i,j,M,N);
	//			cout<<tmp<<" ";
				arr.push_back(rect[i][j]);
			}
	//		cout<<endl;
		}
		int mp[512*512+1];
		for(i=1;i<=512*512;++i)
			mp[i]=0;
		int p,q;
		int tmpsz;
		for(tmpsz=M*N;tmpsz>=1;--tmpsz)
		{
			for(i=0;i<M;++i)
			{
				for(j=0;j<N;++j)
				{
					if(!rect[i][j].gone && rect[i][j].sz>=tmpsz)
					{
							bool ok=true;
							for(p=i;p<i+tmpsz;++p)
							{
								for(q=j;q<j+tmpsz;++q)
								{
									if(rect[p][q].gone==true)
										ok=false;
								}
							}
							if(ok)
							{
								mp[tmpsz]++;
								for(p=i;p<i+tmpsz;++p)
								{
									for(q=j;q<j+tmpsz;++q)
									{
										rect[p][q].gone=true;
									
									}
								}
							}
					}
				}
			}
		}
					
		/*sort(arr.begin(),arr.end());
		
		int arrsz=arr.size();
		int mp[512*512+1];
		for(i=1;i<=512*512;++i)
			mp[i]=0;
		for(i=0;i<arrsz;++i)
		{
			node nd=arr[i];
			if(!rect[arr[i].r][arr[i].c].gone)
			{
				int tmpsz;
				for(tmpsz=arr[i].sz;tmpsz>=1;--tmpsz)
				{
					bool ok=true;
					for(j=arr[i].r;j<arr[i].r+tmpsz;++j)
					{
						for(k=arr[i].c;k<arr[i].c+tmpsz;++k)
						{
							if(rect[j][k].gone==true)
								ok=false;
						}
					}
					if(ok)
					{
						mp[tmpsz]++;
						for(j=arr[i].r;j<arr[i].r+tmpsz;++j)
						{
							for(k=arr[i].c;k<arr[i].c+tmpsz;++k)
							{
								rect[j][k].gone=true;
							
							}
						}
						break;
					}
				}
			}
		}*/
		int count=0;
		for(i=1;i<=512*512;++i)
			if(mp[i]>0)
				++count;
		fout<<count<<endl;
		for(i=512*512;i>=1;--i)
			if(mp[i]>0)
				fout<<i<<" "<<mp[i]<<endl;
	}
	return 0;
}
