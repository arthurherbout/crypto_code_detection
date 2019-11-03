#include <iostream>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <cstdio>
#include <limits>
#include <set>
#include <map>
#include <cassert>
#include <fstream>
#include <queue>
#include <cstring>

using namespace std;

typedef pair<double, int> ii;

int M = 33;
int N;
int mas[1005];
vector<double> v;

vector< double > ans;
double X[1005];
int cnt[100];

int main()
{
	int test_cnt;
	cin >> test_cnt;
	for(int test_num=1;test_num<=test_cnt;test_num++)
	{
		v.resize(0);
		cin >> N;
		for(int i=0;i<N;i++)
			scanf("%d", &mas[i]);
		for(int i=0;i<N-1;i++)
		{
			int t = -1;
			for(int j=0;j<N;j++)
				if (mas[j] == i) {t = j; break;}
			if (t==-1) abort();
			swap(mas[i], mas[t]);
			v.push_back(double(t-i)/(N-i-1));
		}
		sort(v.begin(), v.end());
		double p1 = 1.0/M;
		double x = 0;
		memset(cnt, 0, M*sizeof(int));
		for(int i=0,j=0;i<N;i++)
		{
			while(v[i]>double(j+1)/M) j++;
			cnt[j]++;
		}
		for(int i=0;i<M;i++)
			x += pow(double(cnt[i])/N - p1, 2);
				
		ans.push_back(x);
		X[test_num] = x;
	}
	sort(ans.begin(), ans.end());
	double x = ans[test_cnt/2];
	for(int test_num=1;test_num<=test_cnt;test_num++)
	{
		if (X[test_num]<x) printf("Case #%d: GOOD\n", test_num);
		else printf("Case #%d: BAD\n", test_num);
	}
	return 0;
}
