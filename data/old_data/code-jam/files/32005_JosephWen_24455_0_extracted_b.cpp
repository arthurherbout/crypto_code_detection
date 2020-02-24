#include <cstdio>
#include <algorithm>

using namespace std;

int MAP[10][10];
int X, Y, MX, MY;

int setAttack;
void attack( int org[10][10], int tmp[10][10], int x, int y)
{
	int m, mx=0, my=0;
	m = 0;
	if( org[x][y] == 0)
		return;
	for(int i=y-1; i>=0; i--){
		if( org[x][i] > m ){
			mx = x;
			my = i;
			m = org[x][i];
			break;
		}
		if( org[x][i] > 0 )
			break;
	}
	for(int i=y+1; i<Y; i++){
		if( org[x][i] > m ){
			mx = x;
			my = i;
			m = org[x][i];
			break;
		}
		if( org[x][i] > 0 )
			break;
	}
	for(int i=x-1; i>=0; i--){
		if( org[i][y] > m ){
			mx = i;
			my = y;
			m = org[i][y];
			break;
		}
		if( org[i][y] > 0 )
			break;
	}
	for(int i=x+1; i<X; i++){
		if( org[i][y] > m ){
			mx = i;
			my = y;
			m = org[i][y];
			break;
		}
		if( org[i][y] > 0 )
			break;
	}
	
	if( m != 0){
		//printf("attack %d %d at %d %d\n", mx, my, x, y);
		setAttack = 1;
		tmp[mx][my] = max(0, tmp[mx][my] - org[x][y]);
	}
}
int max_day, forever;
void sim( int org[10][10], int day)
{
	int tmp[10][10];
	int tmp2[10][10];
	/*
	printf("%d\n", day);
	for( int j=0;j < Y;j++){
		for( int i=0;i<X;i++){
			printf("%d ", org[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	*/
	if( org[MX][MY] == 0 ) return ;
	max_day = max( max_day , day );
	for( int j=0;j < Y;j++){
		for( int i=0;i<X;i++){
			tmp[i][j] = org[i][j];
		}
	}
	setAttack = 0;
	for( int i=0;i<X;i++)
		for( int j=0;j < Y;j++){
			if( i == MX && j == MY) continue;
			attack( org, tmp, i , j );
		}
	if(setAttack == 0){
		if( org[MX][MY] > 0)
			forever = 1;
		return ;
	}
	
	
	int x = MX, y = MY;
	for( int j=0;j < Y;j++){
		for( int i=0;i<X;i++){
			tmp2[i][j] = tmp[i][j];
		}
	}
	for(int i=y-1; i>=0; i--){
		if( tmp2[x][i] > 0 ){
			tmp2[x][i] = max( 0, tmp2[x][i] - org[x][y]);
			sim( tmp2, day + 1 );
			break;
		}
	}
	for( int j=0;j < Y;j++){
		for( int i=0;i<X;i++){
			tmp2[i][j] = tmp[i][j];
		}
	}
	for(int i=y+1; i<Y; i++){
		if( tmp2[x][i] > 0 ){
			tmp2[x][i] = max( 0, tmp2[x][i] - org[x][y]);
			sim( tmp2, day + 1 );
			break;
		}
	}
	for( int j=0;j < Y;j++){
		for( int i=0;i<X;i++){
			tmp2[i][j] = tmp[i][j];
		}
	}
	for(int i=x+1; i<X; i++){
		if( tmp2[i][y] > 0 ){
			tmp2[i][y] = max( 0, tmp2[i][y] - org[x][y]);
			sim( tmp2, day + 1 );
			break;
		}
	}
	for( int j=0;j < Y;j++){
		for( int i=0;i<X;i++){
			tmp2[i][j] = tmp[i][j];
		}
	}
	for(int i=x-1; i>=0; i--){
		if( tmp2[i][y] > 0 ){
			tmp2[i][y] = max( 0, tmp2[i][y] - org[x][y]);
			sim( tmp2, day + 1 );
			break;
		}
	}
	for( int j=0;j < Y;j++){
		for( int i=0;i<X;i++){
			tmp2[i][j] = tmp[i][j];
		}
	}
	sim( tmp2, day+1);
}
int main()
{
	int ccN;
	scanf("%d", &ccN);
	for( int cc=0; cc < ccN; cc++ ){
		scanf("%d%d%d%d", &X, &Y, &MX, &MY);
		MX--; MY--;
		for( int i=0; i<Y;i++)
			for( int j=0; j<X; j++)
				scanf("%d", &MAP[j][i]);

		max_day = 0;
		forever = 0;
		sim(MAP, 0);
		if(forever == 1) 
			printf("Case #%d: forever\n", cc+1);
		else
			printf("Case #%d: %d day(s)\n", cc+1, max_day);
	}	
	return 0;
}
