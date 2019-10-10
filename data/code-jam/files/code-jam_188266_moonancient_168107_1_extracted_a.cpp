//#define DEBUG

#include <iostream>
#include <algorithm>
using namespace std;

const int size=50000000;
int flag[size];
int flagt;

bool check(int n,int b)
{
	int digits[40];
	int cnt[10];
	int len;
#ifdef DEBUG
	cerr<<n<<' ';
#endif
	flagt++;
	flag[n]=flagt;
	memset(digits,0,sizeof(digits));
	len=0;
	while (n)
	{
		digits[len++]=n%b;
		n/=b;
	}
	bool ans;
	while (true)
	{
		int sum=0;
		bool succ=true;
		memset(cnt,0,sizeof(cnt));
		for (int i=0;i<len;i++)
		{
			cnt[digits[i]]++;
			if (digits[i] && digits[i]!=1) succ=false;
		}
		if (succ && cnt[1]==1)
		{
			ans=true;
			break;
		}
		memset(digits,0,sizeof(digits));
		for (int i=1;i<b;i++)
		{
			int now=i*i*cnt[i];
			sum+=now;
			for (int j=0;now;j++)
			{
				digits[j]+=now%b;
				now/=b;
			}
		}
#ifdef DEBUG
		cerr<<sum<<' ';
#endif
		if (flag[sum]==flagt || sum>=size)
		{
			ans=false;
			break;
		}
		flag[sum]=flagt;
		int olen=len;
		for (len=39;!digits[len];len--);
		for (int i=0;i<len;i++)
		{
			digits[i+1]+=digits[i]/b;
			digits[i]%=b;
		}
		while (digits[len])
		{
			digits[len+1]=digits[len]/b;
			digits[len]%=b;
			len++;
		}
	}
#ifdef DEBUG
	cerr<<endl;
#endif
	return ans;
}

int n;
int bases[10];
int ans;

int main()
{
	flagt=0;
	memset(flag,0,sizeof(flag));
	cout<<"int table[512]={";
	for (int caseI=0;caseI<(1<<9);caseI++)
	{
		n=0;
		for (int i=0;i<9;i++)
		{
			if (caseI&(1<<i))
				bases[n++]=i+2;
		}
		if (caseI==511) ans=11814485;
		else
		for (ans=2;;ans++)
		{
			bool flag=true;
			for (int i=0;i<n && flag;i++)
				if (!check(ans,bases[i]))
					flag=false;
			if (flag) break;
		}
		if (caseI) cout<<',';
		cout<<ans;
		cerr<<caseI<<' '<<flagt<<endl;
	}
	cout<<"};";
	return 0;
}


