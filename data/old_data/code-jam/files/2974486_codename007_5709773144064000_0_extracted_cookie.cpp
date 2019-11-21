#include <iostream>
#include <iomanip>
using namespace std;
int main(int argc, char const *argv[])
{
	int T,t1=1;
	cin>>T;
	while(T--)
	{
		double C,F,X,r=2.0,tt=0.0;
		cin>>C>>F>>X;
		if(C>=X)
		{
			cout<<setprecision(7)<<fixed;
			cout<<"Case #"<<t1<<": "<<X/r<<endl;
		}
		else
		{
			int nol=X/C;
			int i=nol-1;
			while(i--)
			{
				tt=tt+(C/r);
				r=r+F;
			}
			tt=tt+(X/r);
			cout<<setprecision(7)<<fixed;
			cout<<"Case #"<<t1<<": "<<tt<<endl;

		}
		t1++;

	}
	return 0;
}