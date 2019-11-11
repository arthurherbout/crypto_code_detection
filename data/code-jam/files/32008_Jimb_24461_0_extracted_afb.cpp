// Using libUtil from libGlov (Graphics Library of Victory) available at http://bigscreensmallgames.com/libGlov
#include "utils.h"
#include "file.h"
#include "strutil.h"
#include "assert.h"
#include "array.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <conio.h>
#include "rand.h"
#include <vector>
using namespace std;

int N;
int v[1001];
int C[30];

static bool checkit(int loc, int vv, int p)
{
	int cless=0;
	for (int i=0; i<vv; i++)
	{
		cless += C[i];
	}
	for (int i=loc; i<N-1; i+=p)
	{
		int d = v[i+1] - v[i];
		if (d != C[vv] - cless)
			return false;
	}
	return true;
}

int glob_override=0;

int getvalue(int vv, int restrict)
{
	int d = N - restrict;
	int d2 = d+1;
	int v2=0;
	for (int i=0; i<vv+2; i++)
	{
		int b = (1<<i);
		int dd = !!(d2&b) - !!(d&b);
		if (dd && i>=vv)
		{
			glob_override = 1;
			return -1;
		}
		v2 = (v2 + C[i]*dd + 10007) % 10007;
	}
	return (v[N-1] + v2 + 10007) % 10007;
}

static int doit(int vv, int restrict)
{
	int p = 1<<(vv+1);
	if (p > N)
	{
		return getvalue(vv, restrict);
	}
	int cless=0;
	for (int i=0; i<vv; i++)
	{
		cless += C[i];
	}
	int finalr=-1;
	for (int i=restrict-1; i<=restrict-1 + p; i+=p/2)
	{
		if (i==-1)
		{
			C[vv] = v[0];
		} else {
			if (i<0)
				continue;
			C[vv] = v[i+1] - v[i] + cless; // todo: edge
		}
		if (checkit(i, vv, p))
		{
			int r = doit(vv+1, i % p);
			if (r!=-1)
			{
				if (finalr==-1)
					finalr = r;
				else if (finalr!=r)
					return -1;
			}
		}
	}
	return finalr;
}

char *doB(char **&toks)
{
	static char buf[1024];
	N = atoi(*toks++);
	glob_override = 0;
	for (int i=0; i<N; i++)
	{
		v[i] = atoi(*toks++);
	}
	int r = doit(0, 0);
	if (glob_override)
		r = -1;

	if (r==-1)
		strcpy(buf, "UNKNOWN");
	else
		sprintf(buf, "%d", r);
	return buf;
}

