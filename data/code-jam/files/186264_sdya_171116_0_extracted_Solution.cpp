#include <iostream>
#include <string>
#include <string.h>
#include <cstring>

using namespace std;

char s[1000];
char b[100];

int t, tt;
int l, m;
int k = -1, n;


int dt[1000][2];
double p[1000][2];
char word[1000][2][20];
int par[1000], prn[1000];

int v = 0;
int cv = 0;

char anim[20];
char prop[200][20];
int cp;

void transform(char s[1000], int bg, int ed, int pr, int num)
{
    double a = 0;
    double x = 0, y = 0, z = 1;
    int i, pt = 0;
    int s1 = -1, e1 = -1, s2 = -1, e2 = -1;
    char c[20];
    int cd = -1;

    int sk = 0;

    memset(c, 0, sizeof(c));

    for (i = bg + 1; i < ed; i ++)
        if (s[i] - '0' >= 0 && s[i] - '0' <= 9)
        {
            if (pt == 0) x = x * 10.0 + ((double)(s[i] - '0')); else
                y = y * 10.0 + ((double)(s[i] - '0')), z *= 10.0;
        } else
        if (s[i] == '.') pt ++; else
        if (s[i] - 'a' >= 0 && s[i] - 'z' <= 0) cd ++, c[cd] = s[i]; else 
        if (s[i] == '(')
        {
            s1 = i;
            sk ++;
            break;
        }

    if (s1 == -1)
    {
        v ++;
        dt[pr][num] = v;
        p[pr][num] = x + y / z;

        par[v] = pr;
        prn[v] = num;

        for (i = 0; i < 20; i ++)
            word[pr][num][i] = c[i];

        return;
    } else
    {
        for (i = s1 + 1; i < ed; i ++)
            if (s[i] == '(') sk ++; else
            if (s[i] == ')')
            {
                sk --;
                if (sk == 0)
                {
                    e1 = i;
                    break;
                }
            }
        for (i = e1 + 1; i < ed; i ++)
            if (s[i] == '(') 
            {
                sk ++;
                s2 = i;
                break;
            }
        for (i = s2 + 1; i < ed; i ++)
            if (s[i] == '(') sk ++; else
            if (s[i] == ')')
            {
                sk --;
                if (sk == 0) 
                {
                    e2 = i;
                    break;
                }
            }

        v ++;
        dt[pr][num] = v;
        p[pr][num] = x + y / z;

        par[v] = pr;
        prn[v] = num;

        for (i = 0; i < 20; i ++)
            word[pr][num][i] = c[i];

        int vv = v;

        transform(s, s1, e1, vv, 0);
        transform(s, s2, e2, vv, 1);
    }
}

bool have_prop(char s[20])
{
    for (int i = 1; i <= cp; i ++)
    {
        int t = 0;
        for (int j = 0; j < 20; j ++)
            if (s[j] != prop[i][j])
            {
                t = 1;
                break;
            }
        if (t == 0) return true;
    }
    return false;
}

void analis()
{
    double q = 1.0;
    int sv = 0;

    char prp[20];

    sv = 1;
    q *= p[0][0];

    while (true)
    {
        memset(prp, 0, sizeof(prp));
        for (int i = 0; i < 20; i ++) prp[i] = word[par[sv]][prn[sv]][i];

        if (dt[sv][0] == 0 && dt[sv][1] == 0)
        {
            break;
        } else
        {
            if (have_prop(prp)) 
            {
                q *= p[sv][0];
                sv = dt[sv][0];
            } else
            {
                q *= p[sv][1];
                sv = dt[sv][1];
            }
        }
    }

    
    printf("%.7lf\n", q);
}

int main()
{
    freopen("input.txt","r",stdin);
    freopen("output.txt","w",stdout);

    scanf("%d\n", &t);
    for (int tt = 1; tt <= t; tt ++)
    {
        v = 0;
        for (int i = 0; i < 1000; i ++)
            s[i] = 0;
        memset(word, 0, sizeof(word));
        memset(dt, 0, sizeof(dt));
        k = -1;
        scanf("%d\n", &l);
        for (int i = 1; i <= l; i ++)
        {
            memset(b, 0, sizeof(b));
            gets(b);

            n = strlen(b);
            for (int j = 0; j < n; j ++)
                if (b[j] != ' ') k ++, s[k] = b[j]; 
        }

        transform(s, 0, strlen(s) - 1, 0, 0);
        scanf("%d\n", &n);
        printf("Case #%d:\n", tt);
        for (int i = 1; i <= n; i ++)
        {
            memset(anim, 0, sizeof(anim));
            memset(prop, 0, sizeof(prop));

            scanf("%s %d", &anim, &cp);
            for (int j = 1; j <= cp; j ++)
                scanf(" %s", &prop[j]);
            scanf("\n");

            analis();
        }
    }
    return 0;
}