#include <stdio.h>
#include <string.h>

int map[250][250], na, nb;
int link[250], lx[250], vis[250], ansa, ansb; 

int can(int cur)
{ 
	if (vis[cur])
		return 0; 
	vis[cur] = 1; 
	int i, n = na + nb; 
	for (i = 0; i < n; i++) 
	{
		if (map[cur][i] && (link[i] == -1 || can(link[i])))
		{ 
			lx[cur] = i; 
			link[i] = cur; 
			return 1; 
		}
    }
	return 0;
}


int max_match()
{ 
	int i, n = na + nb; 
	memset(link, 0xff, sizeof(link)); 
	memset(lx, 0xff, sizeof(lx)); 
	
    for (i = 0; i < n; i++) 
	{ 
		memset(vis, 0, sizeof(vis)); 
		can(i); 
	}
	
	ansa = ansb = 0; 
	for (i = 0; i < na; i++) 
	{
        if (link[i] == -1) ansa++; 
    }
	for (i = na; i < na + nb; i++) 
    {
        if (link[i] == -1) ansb++; 
    }
}

int ta[200], tb[200], ta1[200], tb1[200];

int main() 
{ 
	freopen("B-large.in", "r", stdin );
    freopen("chfboutl.txt", "w", stdout ); 
	int T, tcnt = 0, i, j, tor; 
	int tmpAh, tmpAm, tmpBh, tmpBm; 
	scanf("%d", &T); 
	while (T--) 
	{ 
		scanf("%d%d%d", &tor, &na, &nb); 
		for (i = 0; i < na; i++) 
		{
			scanf("%d:%d %d:%d", &tmpAh, &tmpAm, &tmpBh, &tmpBm); 
			ta[i] = tmpAh * 60 + tmpAm; 
			tb[i] = tmpBh * 60 + tmpBm; 
		}
		for (i = 0; i < nb; i++) 
		{
			scanf("%d:%d %d:%d", &tmpBh, &tmpBm, &tmpAh, &tmpAm); 
			tb1[i] = tmpAh * 60 + tmpAm;
			ta1[i] = tmpBh * 60 + tmpBm;
		}
		for (i = 0; i < na + nb; i++)
		{
            for (j = 0; j < na + nb; j++) 
			{
                map[i][j] = 0; 
            }
        }
		for (i = 0; i < na; i++) 
		{
			for (j = 0; j < nb; j++) 
			{
				if (tb[i] + tor <= ta1[j]) map[i][j + na] = 1; 
				if (tb1[j] + tor <= ta[i]) map[j + na][i] = 1; 
			}
        }
		max_match();
		printf("Case #%d: %d %d\n", ++tcnt, ansa, ansb); 
	}
	return 0;
} 
