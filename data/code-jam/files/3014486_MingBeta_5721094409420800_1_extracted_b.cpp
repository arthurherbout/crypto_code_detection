#include <iostream>

using namespace std;


void solveit(int a[], int x, int &now, int n)
{
	int nowt = 0;
	for (int i = 1 ; i <= x ; i++)
	{
		int j = i;
		while (j > 0 && a[j] < a[j-1])
		{
			nowt ++;
			if (nowt > now) return;
			int t = a[j];
			a[j] = a[j-1];
			a[j-1] = t;
			j--;
		}
	}

	for (int i = x+1 ; i < n-1 ; i++)
	{
		int j = i;
		while (j < n && a[j] < a[j+1])
		{
			nowt ++;
			if (nowt > now) return;
			int t = a[j];
			a[j] = a[j+1];
			a[j+1] = t;
			j++;
		}		
	}

	if (nowt < now) now = nowt;
}

void solveit0(int a[], int x, int &now, int n)
{
	int nowt = 0;
	for (int i = 1 ; i < x ; i++)
	{
		int j = i;
		while (j > 0 && a[j] < a[j-1])
		{
			nowt ++;
			if (nowt > now) return;
			int t = a[j];
			a[j] = a[j-1];
			a[j-1] = t;
			j--;
		}
	}

	for (int i = x ; i < n-1 ; i++)
	{
		int j = i;
		while (j < n && a[j] < a[j+1])
		{
			nowt ++;
			if (nowt > now) return;
			int t = a[j];
			a[j] = a[j+1];
			a[j+1] = t;
			j++;
		}		
	}
	//for (int j = 0 ; j < n ; j++) cout << a[j] << " "; cout << endl;
	if (nowt < now) now = nowt;
}

int main()
{
	freopen("B-large.in", "r", stdin);
	freopen("B-large.out", "w", stdout);
	int T;
	cin >> T;
	for (int tt = 1 ; tt <= T ; tt++)
	{
		int n;
		int a[1001];
		int maxnum = 0, maxi;
		cin >> n;
		for (int i = 0 ; i < n ; i++)
		{
			cin >> a[i];
			if (a[i] > maxnum)
			{
				maxnum = a[i];
				maxi = i;
			}
		}
		/*
		//cout << "maxi " << maxi << endl;
		int b[1001];
		int maxt = n * n;

		for (int i = 0 ; i < n ; i++)
		{
			//for (int j = 0 ; j < n ; j++) cout << a[j] << " "; cout << endl;
			//cout << "after ";
			//for (int j = 0 ; j < n ; j++) cout << a[j] << " "; cout << endl;
			for (int j = 0 ; j < n ; j++) b[j] = a[j];
			solveit(b, i, maxt, n);
			//cout << "anser    " << i << " " << maxt << endl;
			//cout << "a ";
			//for (int j = 0 ; j < n ; j++) cout << a[j] << " "; cout << endl;
			//cout << "b " << endl;

			for (int j = 0 ; j < n ; j++) b[j] = a[j];
			solveit0(b, i, maxt, n);
			//cout << "c " << endl;
		}
		*/

		int l[1001], r[1001];
		memset(l, 0, sizeof(l));
		memset(r, 0, sizeof(r));

		for (int i = 0 ; i < n ; i++)
		{
			for (int j = i-1 ; j>=0 ; j--)
				if (a[j] > a[i]) l[i] ++;
			for (int j = i+1 ; j < n ; j++)
				if (a[j] > a[i]) r[i] ++;
		}

		int maxt = 0;
		for (int i = 0 ; i < n ; i++)
			if (l[i] > r[i]) maxt += r[i];
				else maxt += l[i];
		cout << "Case #" << tt << ": " << maxt << endl;
	}
}