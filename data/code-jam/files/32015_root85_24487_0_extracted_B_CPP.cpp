#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <cmath>

#pragma comment(linker, "/STACK:16777216")

using namespace std;

string s;

long long res = 0;
int n;

long long pows10[20];

long long tolong(string &s)
{
	long long r = 0;

	/*stringstream ss;
	ss <<s;
	ss >>r;*/
	for (int i = s.length() - 1;i >= 0;i--)
		r += pows10[s.length() - 1 - i] * (long long)(s[i] - '0');

	return r;
}

void main()
{

	long long ppp = 1;
	for (int i = 0;i < 20;i++)
	{
		pows10[i] = ppp;
		ppp *= 10;
	}

	ifstream in("B-small-attempt0.in");
	ofstream out("B.out");

	int tc;
	in >>tc;
	for (int cc = 0;cc < tc;cc++)
	{
		string s;
		in >>s;
		n = s.length();


		int lim = (int)(pow(3.0, (double)(n - 1)) + 0.5);

		int res = 0;
		for (int t = 0;t < lim;t++)
		{
			char buf[100];
			itoa(t, buf, 3);

			string p = buf;
			while (p.length() < n - 1)
				p = "0" + p;

			long long cur = 0;
			string str;
			str += s[0];

			int k = 0;
			while (k < n - 1 && p[k] == '0')
			{
				str += s[k + 1];
				k++;
			}
			
			cur = tolong(str);

			// 0 - nothing, 1 - minus, 2 - plus

			while (k < n - 1)
			{
				int oper = p[k] - '0';
				k++;

				str.clear();
				str += s[k];

				while (k < n - 1 && p[k] == '0')
				{
					str += s[k + 1];
					k++;
				}

				long long tmp = tolong(str);
				if (oper == 1) // minus
					cur -= tmp;
				else
					cur += tmp;
			}

			if (cur % 2 == 0 || cur % 3 == 0 || cur % 5 == 0 || cur % 7 == 0)
				res++;
		}

		out <<"Case #"<<cc + 1<<": "<<res<<endl;
	}
}
