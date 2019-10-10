//#include <iostream>
//#include <string>
//#include <algorithm>
//#include <vector>
//#include <map>
//
//
//using namespace std;
//const bool DB = true;
//const int MAXINT = 10000000;
//#define MIN(a, b) a=a<b?a:b
//#define MAX(a, b) a=a<b?b:a
//
//vector<int> vh;
//vector<int> vw;
//int main()
//{
//	int t;
////	freopen("A-small-attempt4.in", "r", stdin);
////	freopen("A-small-attempt42.out",  "w", stdout);
//	scanf("%d", &t);
//	for (int ctr = 1; ctr <= t; ctr++)
//	{
//		vh.clear();
//		vw.clear();
//		printf("Case #%d: \n", ctr);
//		int N, M;
//		cin>>N;
//		int H, W;
//		string X;
//		int minh = MAXINT, maxh = -1, minw = MAXINT, maxw = -1;
//		int h1 = -1, h2 = MAXINT,  w1 = -1, w2 = MAXINT;
//
//		for (int i = 0;i < N; i++)
//		{
//			cin>>H>>W>>X;
//			if (X == "BIRD")
//			{
//				MIN(minh, H);
//				MAX(maxh, H);
//				MIN(minw, W);
//				MAX(maxw, W);
//			}
//			else
//			{
//				cin>>X;
//				vh.push_back(H);
//				vw.push_back(W);
//
//			/*	if (H > minh)	MIN(h2, H);
//				if (H < maxh) MAX(h1, H);
//				if (W < minw) MAX(w1,  W);
//				if (W > maxw) MIN(w2, W);*/
//			}
//		}
//	
//		for (int i = 0; i < vh.size(); i++)
//		{
//			if (minh <= maxh)
//				if (! (vh[i] >= minh && vh[i] <= maxh))
//				{
//					if (vh[i] > minh)	MIN(h2, vh[i]);
//					if (vh[i] < maxh) MAX(h1, vh[i]);
//				}
//
//
//			if (minw <= maxw)
//				if (! (vw[i] >= minw && vw[i] <= maxw))
//				{
//					if (vw[i] > maxw) MIN(w2, vw[i]);
//					if (vw[i] < minw) MAX(w1,  vw[i]);
//				}
//		}
//
//		cin>>M;
//		if (DB)
//		{
//			printf("minh = %d, maxh = %d, minw = %d, maxw = %d\n", minh, maxh, minw, maxw);
//			printf("h1 = %d, h2 = %d, w1 = %d, w2 = %d\n", h1, h2, w1, w2);
//		}
//		for (int i = 0; i < M; i++)
//		{
//
//			cin>>H>>W;
//			if (H>= minh && H <= maxh && W >= minw && W <= maxw)
//			{
//				cout<<"BIRD"<<endl;
//			}
//			else
//			{
//				if (( (H <= h1 ||  H >= h2) && minh <= maxh) || (( W <= w1 || W >= w2) && (minw < maxw)))
//					cout<<"NOT BIRD"<<endl;
//				else
//					cout<<"UNKNOWN"<<endl;
//			}
//		}
//	}
//	return 0;
//}

#include <iostream>
#include <string>
using namespace std;

const int MAX = 1000000;

int fh[1000010], fw[1000010];
int N, M;
int main()
{
	int t;
	freopen("A-small-attempt7.in", "r", stdin);
	freopen("A-small-attempt7.out",  "w", stdout);
	cin>>t;
	int i, j;
	for (int ctr = 1; ctr<= t; ctr++)
	{
		cout<<"Case #"<<ctr<<':'<<endl;
		cin>>N;
		int H, W;
		string X;
		memset(fh, 0, sizeof(fh));
		memset(fw, 0, sizeof(fw));
		for (i = 0; i < N; i++)
		{
			cin>>H>>W>>X;
			if (X == "BIRD")
			{
				fh[H] = 1, fw[W] = 1;
			}
			else
			{
				cin>>X;
				fh[H] = -1, fw[W] = -1;
			}
		}

		i = 0, j = MAX;
		while (i <= j && fh[i] != 1) i++;
		while (i <= j && fh[j] != 1) j--;
		if (fh[i] == 1 && fh[j] == 1)
		{
			for (int k = i; k <= j; k++)
				fh[k] = 1;
			while (i >= 0 && fh[i] != -1) i--;
			while (j <= MAX && fh[j] != -1) j++;
			for (int k = 0; k <= i; k++)
				fh[k] = -1;
			for (int k = j; k <= MAX; k++)
				fh[k] = -1;
		}

	    i = 0, j = MAX;
		while (i <= j && fw[i] != 1) i++;
		while (i <= j && fw[j] != 1) j--;
		if (fw[i] == 1 && fw[j] == 1)
		{
			for (int k = i; k <= j; k++)
				fw[k] = 1;
			while (i >= 0 && fw[i] != -1) i--;
			while (j <= MAX && fw[j] != -1) j++;
			for (int k = 0; k <= i; k++)
				fw[k] = -1;
			for (int k = j; k <= MAX; k++)
				fw[k] = -1;
		}

		cin>>M;
		for (i = 0; i < M; i++)
		{
			cin>>H>>W;
			if (fh[H] == -1 || fw[W] == -1)
				cout<<"NOT BIRD"<<endl;
			else
				if (fh[H] == 1 && fw[W] == 1)
					cout<<"BIRD"<<endl;
				else
					cout<<"UNKNOWN"<<endl;
		}


	}
	return 0;
}