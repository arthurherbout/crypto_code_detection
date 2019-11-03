#include<iostream>

#include <cstring>
#include <string.h>
#include <map>
#include <deque>
#include <queue>
#include <stack>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <vector>
#include <set>
#include <complex>
#include <list>




using namespace std;

class pswpro
{
public:

	void getstring()
	{
		int casenum;
		cin>>casenum;
		for(int ii=1;ii<=casenum;ii++)
		{
			double result=0;
			int A,B;
			cin>>A>>B;
			vector<double> cor;
		
			for(int j=1;j<=A;j++)
			{
				double t;
				cin>>t;
				cor.push_back(t);
			}
			double d1=1;//继续输入
			for(int j=0;j<=cor.size()-1;j++)
			{
				d1*=cor[j];
			}
			d1=d1*(B-A+1)+(1-d1)*(B+1+B-A+1);
			double d2=B+2;//直接按enter
			vector<double> p;//前n个全对的概率
			vector<double> back;
			double t=1;
			for(int i=0;i<=A-1;i++)
			{
				t*=cor[i];
				p.push_back(t);
			}
		  for(int i=1;i<=A-1;i++)
		  {
			  double t=0;
			  t+=p[A-i-1]*(B-A+i+i+1)+(1-p[A-i-1])*(B-A+i+i+B+1+1);
			  back.push_back(t);
		  }
		  sort(back.begin(),back.end());
		  if(d1>d2)
		  {
			  d1=d2;
		  }
		  if(back.size()>0 && back[0]<d1)
		  {
			  d1=back[0];
		  }
		  result=d1;
			cout<<"Case #"<<ii<<": "<<result<<endl;
		}

	}
};

class KRush
{
public:

	void getstring()
	{
		int casenum;
		cin>>casenum;
		for(int ii=1;ii<=casenum;ii++)
		{
			double result=0;
			int N;
			cin>>N;
			vector<int> one,two,complete;
			int star=0;
			for(int i=1;i<=N;i++)
			{
				int t;
				cin>>t;
				one.push_back(t);
				cin>>t;
				two.push_back(t);
				complete.push_back(0);
			}
		   //初始化完成
			while(true)
			{
				bool flag=false;
				for(int i=0;i<=N-1;i++)
				{
					int t=two[i];
					if(t<=star && complete[i]==0)
					{
						
						star+=2;
						complete[i]=2;
						result++;
						flag=true;
						break;
					}
				}
				if(flag)
				{
					continue;
				}
				for(int i=0;i<=N-1;i++)
				{
					int t=two[i];
					if(t<=star && complete[i]==1)
					{
						
						star+=1;
						complete[i]=2;
						result++;
						flag=true;
						break;
					}
				}
				if(flag)
				{
					continue;
				}
				int maxtwo=-1;
				int pos=-1;
				for(int i=0;i<=N-1;i++)
				{
					int t=one[i];
					if(t<=star && complete[i]==0 && two[i]>maxtwo)
					{
						pos=i;
						maxtwo=two[i];
					}
				}
				if(maxtwo!=-1)
				{
					complete[pos]=1;
					star++;
					result++;
					flag=true;
				}
				if(flag==false)
				{
					break;
				}
			}
			bool flag=true;
			for(int i=0;i<=N-1;i++)
			{
				if(complete[i]!=2)
				{
					cout<<"Case #"<<ii<<": "<<"Too Bad"<<endl;
					flag=false;
					break;
				}
			}
			if(flag)
			{
			cout<<"Case #"<<ii<<": "<<result<<endl;
			}
		}

	}
};


class SIN
{
public:
	void getstring()
	{
		int casenum;
		cin>>casenum;
		for(int ii=1;ii<=casenum;ii++)
		{
			vector<double> result;
			int num;
			cin>>num;
			vector<long> score;
			long X=0;
			for(int i=1;i<=num;i++)
			{
				int j;
				cin>>j;
				score.push_back(j);
				X+=j;
				result.push_back(-1);
			}
			/*for(int i=0;i<=num-1;i++)
			{
				long m=getmin(score,i);

				result.push_back((double)(X+m-score[i])/(double)(2*X));
			}*/
			double mean=(double)((double)2*X/(double)num);
			for(int i=0;i<=num-1;i++)
			{
				
				if(score[i]>mean)
				result[i]=0;
			}
			mean=0;
			int sum=0;
			for(int i=0;i<=num-1;i++)
			{
				if(result[i]!=0)
				{
					sum++;
					mean+=score[i];
				}
			}
			mean=(mean+X)/sum;
			for(int i=0;i<=num-1;i++)
			{
				if(result[i]!=0)
				{
					result[i]=(double)(mean-score[i])/X;
				}
			}
			cout<<"Case #"<<ii<<": ";
			for(int i=0;i<=num-1;i++)
			{
				cout<<result[i]*100<<" ";
			}
			cout<<endl;
		}

	}
	long getmin(vector<long> score,int pos)
	{
		long m=999999999;
		
		for(int i=0;i<=score.size()-1;i++)
		{
			if(i!=pos && score[i]<m)
			{
				m=score[i];
			}
		}
		return m;
	}

};

class Sums
{
public:
	vector<int> choice;
	vector<int> nchoice;
	vector<long long> arr;
	long long SUM;
	void getstring()
	{
		int casenum;
		cin>>casenum;
		for(int ii=1;ii<=casenum;ii++)
		{
			double result=0;
			double num;
			cin>>num;
			arr.clear();
			choice.clear();
			nchoice.clear();
			SUM=0;
			for(int i=1;i<=num;i++)
			{
				long long t;
				cin>>t;
				arr.push_back(t);
				choice.push_back(0);
				nchoice.push_back(0);
			}
		//	sort(arr.begin(),arr.end(),greater<long long>());
			if(dp1(0)==false)
			{
			cout<<"Case #"<<ii<<": "<<endl;
			cout<<"Impossible"<<endl;
			}
			else
			{
				cout<<"Case #"<<ii<<": "<<endl;
				for(int i=0;i<=arr.size()-1;i++)
				{
					if(choice[i]!=0)
						cout<<arr[i]<<" ";
				}
				cout<<endl;
				for(int i=0;i<=arr.size()-1;i++)
				{
					if(nchoice[i]!=0)
						cout<<arr[i]<<" ";
				}
				cout<<endl;
			}
		}

	}
	bool dp1(int s)
	{
		if(s>arr.size()-1)
			return false;
		bool flag=false;
		choice[s]=0;
		if(haschoice(SUM,0)==true)
		{
			return true;
		}
	  flag=dp1(s+1);
	  if(flag)
		  return true;
		choice[s]=1;
		SUM+=arr[s];
		if(haschoice(SUM,0))
		{
			return true;
		}
		flag=dp1(s+1);
		SUM-=arr[s];
		choice[s]=0;
		return flag;
	}
	bool haschoice(long long sum,int s)
	{

		if(sum<arr[0] || s>arr.size()-1)
		{
			return false;
		}
		nchoice[s]=1;
		if(arr[s]==sum && mequal()==false)
		{
			
			return true;
		}
		nchoice[s]=0;
		if(s==arr.size()-1)
		{
			return false;
		}
		if(haschoice(sum,s+1)==true)
		{
			
			return true;
		}
		nchoice[s]=1;
		if(haschoice(sum-arr[s],s+1)==true)
		{
		
			return true;
		}
		nchoice[s]=0;
		return false;
	}
	bool mequal()
	{
		for(int i=0;i<=arr.size()-1;i++)
		{
			if(choice[i]!=nchoice[i])
				return false;
		}
		return true;
	}
	//vector<string> dp(vector<long long> a,long long cha)
	//{
	//	if(a.size()==2)
	//	{
	//		if(a[1]-a[0]==cha)
	//		{
	//			vector<string> result;
	//			result.push_back(a[0]);
	//			result.push_back(a[1]);
	//			return result;
	//		}
	//		else
	//		{
	//			return NULL;
	//		}
	//	}
	//	for(int i=0;i<=a.size()-1;i++)
	//	{
	//		long long 
	//	}
	//}

};



class C2
{
public:
	int N;
	void getstring()
	{
		int casenum;
		cin>>casenum;
		
		for(int ii=1;ii<=casenum;ii++)
		{
			double D;
			cin>>D;
			int N,A;
			cin>>N>>A;
			vector<double> time;
			vector<double> pos;
			vector<double> acce;
			for(int i=1;i<=N;i++)
			{
				double t;
				cin>>t;
				time.push_back(t);
				cin>>t;
				pos.push_back(t);
			}
			for(int i=1;i<=A;i++)
			{
				double t;
				cin>>t;
				acce.push_back(t);
			}
			cout<<"Case #"<<ii<<": "<<endl;
			for(int j=0;j<=A-1;j++)
			{
				double v=0;
			    double t=0;
			    double d=0;
				double a=acce[j];
				for(int i=0;i<=N-2;i++)
				{
					t=time[i]-t;
					double dt=d;
					d=d+v*t+0.5*a*t*t;
					
					if(d>pos[i])
					{
						d=pos[i];
						double vt=sqrt((d-dt)*2*a+v*v);
							v=(pos[i+1]-pos[i])/(time[i+1]-time[i]);
						if(vt<v)
						{
							v=vt;
						}
					}
					else
					{
						v=v+a*t;
					}
				
				}
				double rtime,ta,tb,v1;
				/*if(v>(pos[N-1]-pos[N-2])/(time[N-1]-time[N-2]))
				{
					v=(pos[N-1]-pos[N-2])/(time[N-1]-time[N-2]);
					rtime=time[N-2]+(D-pos[N-2])/v;
				}
				else 
				{
					double v1=(pos[N-1]-pos[N-2])/(time[N-1]-time[N-2]);
					double vx=sqrt(2*a*(D-pos[N-2])+v*v);
					if(vx>v1)
					{
						rtime=time[N-2]+(D-pos[N-2])/v1;
					}
					else
					{
						rtime=(vx-v)/a;
					}

				}*/
				if(N!=1)
				{
				double vx=sqrt(2*a*(D-d)+v*v);
				v1=(pos[N-1]-pos[N-2])/(time[N-1]-time[N-2]);
				tb=(D-pos[N-2])/v1;
				ta=(vx-v)/a;
				if(ta>tb)
				{
					rtime=time[N-2]+ta;
				}
				else
				{
					rtime=time[N-2]+tb;
				}
				}
				else
				{
					double vx=sqrt(2*a*(D-d)+v*v);
				   tb=time[0];
				    ta=(vx-v)/a;
					if(ta>tb)
					{
						rtime=ta;
					}
					else
					{
						rtime=tb;
					}
				}
				cout<<fixed << setprecision(6)<<rtime<<endl;;
			}


			
			
		}

	}


};

int main()
{
	freopen("E:/in.in","rt",stdin);
    freopen("E:/out.out","wt",stdout);
	C2 p;
	p.getstring();
	int i;
	cin>>i;
	return 0;
}