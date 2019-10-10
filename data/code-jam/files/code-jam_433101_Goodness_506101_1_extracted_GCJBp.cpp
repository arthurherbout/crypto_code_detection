#include <iostream> 
using namespace std;
#define maxsize 100

struct hp
{
	int len;
	int s[maxsize+1];
}in[1100];

void input(hp &a,string str)
{
	int i;
	while(str[0]=='0' && str.size()!=1)
		str.erase(0,1);
	a.len=(int)str.size();
	for(i=1;i<=a.len;++i)
		a.s[i]=str[a.len-i]-48;
	for (i=a.len+1;i<=maxsize;++i)
		a.s[i]=0;
}


void print(const hp &y)
{
	int i;
	for(i=y.len;i>=1;i--)
  		cout<<y.s[i];
	cout<<endl;
}

int compare(const hp &a,const hp &b)
{
	int len;
	if(a.len>b.len)
		len=a.len;
	else
		len=b.len;
	while(len>0 && a.s[len]==b.s[len]) len--;
	if(len==0) return 0;
	else return a.s[len]-b.s[len];
}

void subtract(const hp &a,const hp &b,hp &c)
{
	int i,len;
	for(i=1;i<=maxsize;i++) c.s[i]=0;
	if(a.len>b.len) len=a.len;
	else len=b.len;
	for(i=1;i<=len;i++)
	{
		c.s[i]+=a.s[i]-b.s[i];
		if(c.s[i]<0)
		{
			c.s[i]+=10;
			c.s[i+1]--;
		}
	}
	while(len>1&&c.s[len]==0) len--;
	c.len=len;
}

void multiply10(hp &a)
{
	int i;
	for(i=a.len;i>=1;i--)
		a.s[i+1]=a.s[i];
	a.s[1]=0;
	a.len++;
	while(a.len>1&&a.s[a.len]==0) a.len--;
}

void divideh(const hp &a,const hp &b,hp &c,hp &d)
{
	hp e;
	int i,len;
	for(i=1;i<=maxsize;i++)
	{
		c.s[i]=0;
		d.s[i]=0;
	}
	len=a.len;
	d.len=1;
	for(i=len;i>=1;i--)
	{
		multiply10(d);
		d.s[1]=a.s[i];
		while(compare(d,b)>=0)
		{
			subtract(d,b,e);
			d=e;
			c.s[i]++;
		}
	}
	while(len>1&&c.s[len]==0) len--;
	c.len=len;
} 


void gcd(const hp &a,const hp &b,hp &c)
{
	string zero="0";
	hp z;
	input(z,zero);
	if(compare(z,b))
	{
		hp e,d;
		divideh(a,b,e,d);
		gcd(b,d,c);
	}
	else c=a;

}

int main()
{
	freopen("B-large.in","r",stdin);
	freopen("B-large.out","w",stdout);
	string get;
	int c,cs,n,i,j;
	cin>>c;
	for(cs=1;cs<=c;cs++)
	{
		cin>>n;
		for(i=0;i<n;i++)
		{
			cin>>get;
			input(in[i],get);
		}
		hp div,theSu,toDiv;
		if(compare(in[0],in[1])>=0) subtract(in[0],in[1],div);
		else subtract(in[1],in[0],div);
		for(i=0;i<n;i++)
		{
			for(j=i+1;j<n;j++)
			{
				if(compare(in[i],in[j])>=0)
					subtract(in[i],in[j],theSu);
				else subtract(in[j],in[i],theSu);
				gcd(div,theSu,toDiv);
				div=toDiv;
			}
		}
		hp ans1,ans2,result;
		divideh(in[0],div,ans1,ans2);
		hp z;
		input(z,"0");
		cout<<"Case #"<<cs<<": ";
		if(compare(ans2,z))
		{
			subtract(div,ans2,result);
			print(result);
		}
		else cout<<"0"<<endl;
	}
}
