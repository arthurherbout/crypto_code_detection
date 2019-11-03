#include <cstdio>
int main()
{
	int d;
	char t[5][5];
	scanf("%d\n", &d);
	for(int k=1; k<=d; k++)
	{
		printf("Case #%d: ", k);
		int dot=0, T=0, X=0, O=0;
		bool c=false;
		for(int i=0; i<5; i++) gets(t[i]);
		for(int i=0; i<4; i++)
		{
			T=0; X=0; O=0;
			for(int j=0; j<4; j++)
			{
				if(t[i][j]=='X') X++;
				else if(t[i][j]=='O') O++;
				else if(t[i][j]=='T') T++;
				else dot++;
			}
			if(X+T==4) { printf("X won\n"); c=true; break; }
			else if(O+T==4) { printf("O won\n"); c=true; break; }
			T=0; X=0; O=0;
			for(int j=0; j<4; j++)
			{
				if(t[j][i]=='X') X++;
				else if(t[j][i]=='O') O++;
				else if(t[j][i]=='T') T++;
				else dot++;
			}
			if(X+T==4) { printf("X won\n"); c=true; break; }
			else if(O+T==4) { printf("O won\n"); c=true; break; }
		}
		if(c) continue;
		T=0; X=0; O=0;
		for(int i=0; i<4; i++)
		{
			if(t[i][i]=='X') X++;
			else if(t[i][i]=='O') O++;
			else if(t[i][i]=='T') T++;
			else dot++;
		}
		if(X+T==4) { printf("X won\n"); c=true; }
		else if(O+T==4) { printf("O won\n"); c=true; }
		if(c) continue;
		T=0; X=0; O=0;
		for(int i=0; i<4; i++)
		{
			if(t[i][3-i]=='X') X++;
			else if(t[i][3-i]=='O') O++;
			else if(t[i][3-i]=='T') T++;
			else dot++;
		}
		if(X+T==4) { printf("X won\n"); c=true; }
		else if(O+T==4) { printf("O won\n"); c=true; }
		if(!c)
		{
			if(dot>0) printf("Game has not completed\n");
			else printf("Draw\n");
		}
	}
	return 0;
}

