#include <cstdio>
#include <iostream>
#include <set>
#include <map>
#include <sstream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <utility>

using namespace std;

#define FOR(i,n) for((i) = 0; (i) < (n); ++(i))
#define SFOR(i,m,n) for((i) = (m); (i) < (n); ++(i))

struct Node
{
	Node *a, *b, *p;
	string prop;
	double w;
} *root;

Node* newNode()
{
	Node *n = new Node();
	n->a = 0;
	n->b = 0;
	n->p = 0;	
	return n;
}

int wmain(void)
{
	FILE *fi, *fo;	
	fi = fopen("a.in", "rt");
	fo = fopen("a.out", "wt");

	char buf[10000];
	
	int t, tt;
	fscanf(fi, "%d", &tt);
	FOR(t, tt)
	{
		int n,m,i,j,l;
		fscanf(fi, "%d\n", &l);
		string s;
		FOR(i,l)
		{
			fgets(buf, 10000, fi);

			s += string(buf) + " ";

		}

		root = newNode();
		Node *cur = root;
		Node *q;
		FOR(i, s.length())
		{
			if (s[i] == '(')
			{
				q = newNode();
				q->p = cur;
				if (cur->a == 0) cur->a = q;
				else cur->b = q;
				cur = q;

				++i;
				j = 0;
				memset(buf, 0, sizeof(buf));
				while (s[i] != ')' && s[i] != '(')
				{
					buf[j++] = s[i];
					++i;
				}
				double w;
				char PROP[10000];
				memset(PROP,0,sizeof(PROP));
				sscanf(buf, "%lf%s", &w, PROP);
				string prop(PROP);
				--i;

				cur->w = w;
				cur->prop = prop;
			}
			else if (s[i] == ')')
			{
				cur = cur->p;
			}
		}


		fscanf(fi, "%d", &n);

		fprintf(fo, "Case #%d:\n", t+1);
		FOR(i, n)
		{
			set<string> P;
	
			fscanf(fi, "%s", &buf);
			string name(buf);

			fscanf(fi, "%d", &m);
			FOR(j, m)
			{
				fscanf(fi, "%s", &buf);
				string prop(buf);
				P.insert(prop);
			}

			double res = 1.0;
			cur = root->a;
			while (cur != 0)
			{
				res *= cur->w;
				if (P.find(cur->prop) != P.end()) cur = cur->a; else cur = cur->b;
			}
			fprintf(fo, "%lf\n", res);
		}
	}

	fclose(fi);
	fclose(fo);

	return 0;
}