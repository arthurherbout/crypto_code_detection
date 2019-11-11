#include <stdio.h>
#include <algorithm>
#include <vector>

using namespace std;

class point {
public:
    int x, y;
    point():x(100),y(100){};
    point(int tx, int ty){x = tx;y = ty;}
};

bool cmp_x(point a, point b)
{
    return (a.x < b.x);
}

bool cmp_y(point a, point b)
{
    return (a.y < b.y);
}

point operator + (point a, point b)
{
    return point(a.x + b.x, a.y + b.y);
}

FILE *inf, *outf;
int n, l, t;
char s[40];
point w[4] = {point(1, 0), point(0, 1), point(-1, 0), point(0, -1)};
point prof;
int head;
int flag, lim, chk;
int cnt;
int f[210][210];

int main()
{
    int l1, l2, l3, l4;
    inf = fopen("a.in", "r");
    outf = fopen("a.out", "w");
    fscanf(inf, "%d", &n);
    for (l1 = 1; l1 <= n; l1++)
    {
	prof = point();
	head = 1;
	f[prof.x][prof.y] = 1;
	fscanf(inf, "%d", &l);
	for (l2 = 0; l2 <= 201; l2++)
	    for (l3 = 0; l3 <= 201; l3++)
		f[l2][l3] = 0;
	for (l2 = 1; l2 <= l; l2++)
	{
	    fscanf(inf, "%s%d", s, &t);
	    for (l3 = 1; l3 <= t; l3++)
		for (l4 = 0; s[l4] != char(NULL); l4++)
		    if (s[l4] == 'L')
		    {
			head--;
			if (head < 0)
			    head = 3;
		    }
		    else if (s[l4] == 'R')
		    {
			head++;
			if (head > 3)
			    head = 0;
		    }
		    else
		    {
			prof = prof + w[head];
			f[prof.x][prof.y] = 1;
		    }
	}

/*
	fprintf(outf, "\n");
	for (l3 = 0; l3 <= 201; l3++)
	{
	    for (l4 = 0; l4 <= 201; l4++)
		if (f[l3][l4] == 0)
		    fprintf(outf, "_");
		else
		    fprintf(outf, "%d", f[l3][l4]);
	    fprintf(outf, "\n");
	}
	fprintf(outf, "\n");
*/
	for (l2 = 0; l2 <= 201; l2++)		    //
	{
	    flag = 0;
	    for (l3 = 0; l3 <= 201; l3++)	    //
	    {
		if (flag == 1)
		{
		    if (f[l2][l3] == 1)
		    {
			while (f[l2][l3 + 1] == 1)  //
			    l3++;		    //
			chk = 0;
			for (l4 = l3 + 1; l4 <= 201; l4++)
			{
			    if ((f[l2][l4] == 1 && f[l2][l4 + 1] == 0 && f[l2 + 1][l4] == 1) || (f[l2][l4] == 0 && f[l2][l4 + 1] == 1 && f[l2 + 1][l4 + 1] == 1))
			    {
				chk++;
				if (f[l2][l4 + 2] == 0)
				    l4++;
			    }
			}
			if ((chk & 1))
			    continue;
			flag = 2;
			lim = l3;
		    }
		    else
			f[l2][l3] = 1;
		}
		else
		{   
		    if (f[l2][l3] == 1)
		    {
			if (flag == 2)
			    for (l4 = l3 - 1; l4 > lim; l4--)
				f[l2][l4] = 2;
			flag = 1;
		    }
		}
	    }
	}
	/*
	fprintf(outf, "\n");
	for (l3 = 0; l3 <= 201; l3++)
	{
	    for (l4 = 0; l4 <= 201; l4++)
		if (f[l3][l4] == 0)
		    fprintf(outf, "_");
		else
		    fprintf(outf, "%d", f[l3][l4]);
	    fprintf(outf, "\n");
	}
	fprintf(outf, "\n");
	*/
	for (l2 = 0; l2 <= 201; l2++)		    //
	{
	    flag = 0;
	    for (l3 = 0; l3 <= 201; l3++)	    //
	    {
		if (flag == 1)
		{
		    if (f[l3][l2] == 1)
		    {
			while (f[l3 + 1][l2] == 1)  //
			    l3++;		    //
			chk = 0;
			for (l4 = l3 + 1; l4 <= 201; l4++)
			{
			    if ((f[l4][l2] == 1 && f[l4 + 1][l2] == 0 && f[l4][l2 + 1] == 1) || (f[l4][l2] == 0 && f[l4 + 1][l2] == 1 && f[l4 + 1][l2 + 1] == 1))
			    {
				chk++;
				if (f[l4 + 2][l2] == 0)
				    l4++;
			    }
			}
			if ((chk & 1))
			    continue;
			flag = 2;
			lim = l3;
		    }
		    else
			f[l3][l2] = 1;
		}
		else
		{   
		    if (f[l3][l2] == 1)
		    {
			if (flag == 2)
			    for (l4 = l3 - 1; l4 > lim; l4--)
				f[l4][l2] = 2;
			flag = 1;
		    }
		}
	    }
	}
	cnt = 0;
	for (l3 = 0; l3 <= 200; l3++)
	    for (l4 = 0; l4 <= 200; l4++)
		if (f[l3][l4] == 2 && f[l3-1][l4]==1 && f[l3+1][l4]==1 && f[l3][l4-1]==1 &&f[l3][l4+1]==1)
		    continue;
		if ((f[l3][l4] == 2 || f[l3 + 1][l4] == 2 || f[l3][l4 + 1] == 2 || f[l3 + 1][l4 + 1] == 2) && (f[l3][l4] != 0 && f[l3 + 1][l4] != 0 && f[l3][l4 + 1] != 0 && f[l3 + 1][l4 + 1] != 0)) 
		    cnt++;
	fprintf(outf, "Case #%d: %d\n", l1, cnt);
/*	
	fprintf(outf, "\n");
	for (l3 = 0; l3 <= 201; l3++)
	{
	    for (l4 = 0; l4 <= 201; l4++)
		if (f[l3][l4] == 0)
		    fprintf(outf, "_");
		else
		    fprintf(outf, "%d", f[l3][l4]);
	    fprintf(outf, "\n");
	}
	fprintf(outf, "\n");*/
    }
    fclose(inf);
    fclose(outf);
    return 0;
}
