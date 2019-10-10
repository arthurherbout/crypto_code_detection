#include <fstream>
#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

int search(int pbegin,int pend,vector<int> myvector)
{
	int min=1000000;
	if (myvector.size()==0) 
		return 0;
	
	if (pend<=pbegin) 
		return 0;

	for (int i=0;i<myvector.size();i++)
	{
		
		vector<int> lvector,rvector;
		vector<int>::iterator iter;
		for (iter=myvector.begin();iter!=myvector.end();iter++)
		{
			int CutAt = myvector[i];
			if((*iter)<CutAt) 
				lvector.push_back(*iter);
			if ((*iter)>CutAt) 
				rvector.push_back(*iter);
		}

		int lszie1= lvector.size();
		int lszie2 = rvector.size();
		int temp1 = search(pbegin,myvector[i]-1,lvector);
		int temp2 = search(myvector[i]+1,pend,rvector);
		
		if (pend-pbegin+temp1+temp2<min) 
			min = pend-pbegin+temp1+temp2;
//		return pend-pbegin+temp1+temp2;
// 			+search(pbegin,myvector[i]-1,lvector)
		//	+search(myvector[i]+1,pend,rvector);
	}

	return min;
}

int main()
{
	filebuf ifb;
	ifb.open ("C-small-attempt0.in",ios::in);
	istream min(&ifb);
	//istream min(min);
	
	filebuf ofb;
	ofb.open ("C-small-attempt0.out.txt",ios::out);
	ostream mout(&ofb);
	//ostream mout(mout);

	int n,i,j;
	min>>n;
	for (i=1;i<=n;i++)
	{
		int p,q;
		min>>p>>q;

		vector<int> myvector;
		for (j=0;j<q;j++)
			{
			int temp;
			min>>temp;
			myvector.push_back(temp);
		}

		
		sort(myvector.begin(),myvector.end());

		int res;
	 	res= search(1,p, myvector);

		mout<<"Case #"<<i<<": "<<res<<endl;
	}
	return 0;
}