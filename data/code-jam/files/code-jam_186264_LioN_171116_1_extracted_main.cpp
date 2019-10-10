#include <cstdio>
#include <cstring>
#include <cstdlib>
#define MAXN 100000
struct node
{
	char name[20];
	int left, right, father;
	double num;
}tree[MAXN];
int tail, now, test, lines, i, j, ani, aa, ok;
char st[MAXN], tmp[MAXN], fea[100][MAXN];
double ret;
int main()
{
	freopen("A-large.in", "r", stdin);
	freopen("A-large.out", "w", stdout);
	scanf("%d", &test);
	for (int tt = 1; tt <= test; tt++)
	{
		scanf("%d", &lines); gets(st);
		memset(st, 0, sizeof(st)); st[0] = 0;
		for (int ll = 1; ll <= lines; ll++) gets(st + strlen(st)), st[strlen(st)] = ' ';
		now = tail = -1;
		for (i = 0; st[i]; )
		{
			while (st[i] && st[i] == ' ' || st[i] == 9) i++;
			if (st[i] == '(')
			{
				if (now == -1)
				{
					tail = now = 0;
					while (!(st[i] >= '0' && st[i] <= '9' || st[i] == '.')) i++;
//					for (j = 0; st[i] >= '0' && st[i] <= '9' || st[i] == '.'; j++, i++) tmp[j] = st[i];
					tree[0].num = atof(st + i); tree[0].left = tree[0].right = tree[0].father = -1;
					while ((st[i] >= '0' && st[i] <= '9' || st[i] == '.')) i++;
					while (st[i] == ' ' || st[i] == 9) i++;
					if (st[i] == ')') tree[0].name[0] = 0;
					else for (j = 0; st[i] >= 'a' && st[i] <= 'z'; j++, i++) tree[0].name[j] = st[i]; tree[0].name[j] = 0;
				}
				else
				{
					tail++;
					while (!(st[i] >= '0' && st[i] <= '9' || st[i] == '.')) i++;
//					for (j = 0; st[i] >= '0' && st[i] <= '9' || st[i] == '.'; j++, i++) tmp[j] = st[i]; tmp[j] = 0;
					tree[tail].num = atof(st + i); tree[tail].left = tree[tail].right = -1; tree[tail].father = now;
					while ((st[i] >= '0' && st[i] <= '9' || st[i] == '.')) i++;
					if (tree[now].left == -1) tree[now].left = tail;
					else tree[now].right = tail;
					now = tail;
					while (st[i] == ' ' || st[i] == 9) i++;
					if (st[i] == ')') tree[tail].name[0] = 0;
					else for (j = 0; st[i] >= 'a' && st[i] <= 'z'; j++, i++) tree[tail].name[j] = st[i]; tree[tail].name[j] = 0;
				}
			}
			if (st[i] == ')') now = tree[now].father, i++;
		}
		printf("Case #%d:\n", tt);
		scanf("%d", &ani);
		for (i = 0; i < ani; i++)
		{
			scanf("%s", st); scanf("%d", &aa);
			for (j = 0; j < aa; j++) scanf("%s", fea[j]);
			ret = 1; now = 0;
			while (now >= 0)
			{
				ok = 0; ret = ret * tree[now].num;
				for (j = 0; j < aa; j++)
					if (strcmp(fea[j], tree[now].name) == 0)
					{
						ok = 1;
						now = tree[now].left;
						break;
					}
				if (!ok) now = tree[now].right;
			}
			printf("%.7lf\n", ret);
		}
	}
	fclose(stdin); fclose(stdout);
	return 0;
}