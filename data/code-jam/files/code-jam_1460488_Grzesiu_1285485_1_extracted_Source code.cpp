#include <cstdio>
#include <cstring>

inline int abs(int a) { return a < 0 ? -a : a; }
inline int sgn(int a) { return a < 0 ? -1 : 1; }

char map[31][31];
int pos_x, pos_y;

bool walk(int _x, int _y) {
	int x = 0;
	int y = 0;
	int X = abs(_x);
	int Y = abs(_y);
	int dx = sgn(_x);
	int dy = sgn(_y);
	int map_x = pos_x;
	int map_y = pos_y;

	while( x != X or y != Y ) {
		int up = (2*y+1)*X;
		int right = (2*x+1)*Y;
		char dir = 0;

		if( up <= right ) {
			dir |= 1;
			y++;
			map_y += dy;
		}
		if( up >= right ) {
			dir |= 2;
			x++;
			map_x += dx;
		}

		if( map[map_y][map_x] == '#' ) {
			if( dir == 3 ) {
				char a = map[map_y][map_x-dx];
				char b = map[map_y-dy][map_x];
				if( a == b ) {
					if( a != '#' ) {
//						puts("zniszczony");
						return false;
					} else {
//						puts("\tróg, zawracam");
					}
				} else {
					if( a == '#' )
						dir = 1;
					else
						dir = 2;
//					puts("\tinny róg");
				}
			}
			if( dir & 1 ) {
				dy = -dy;
				map_y += dy;
			}
			if( dir & 2 ) {
				dx = -dx;
				map_x += dx;
			}
		}

		if( map[map_y][map_x] == 'X' and y*X == x*Y and ( x != X or y != Y ) ) {
//			puts("zaslaniam cel");
			return false;
		}
	}
//	printf("Skonczylem na %c\n", map[map_y][map_x]);
	return map[map_y][map_x] == 'X';
}

void test() {
	int H, W, D;
	scanf("%d %d %d", &H, &W, &D );
	for( int y = 0; y < H; y++ )
		scanf("%s", map[y] );

	for( int y = 0; y < H; y++ )
		for( int x = 0; x < W; x++ )
			if( map[y][x] == 'X' ) {
				pos_x = x;
				pos_y = y;
			}

	int count = -1; // ignore (0,0)
	for( int y = -D; y <= D; y++ )
		for( int x = -D; x <= D; x++ ) 
			if( x*x + y*y <= D*D ) 
				if( walk(x, y) ) {
//					printf("(%d, %d)\n", x, y );
					count++;
				}
	printf("%d\n", count);
}

int main() {
	int T;
	scanf("%d", &T);
	for( int no = 1; no <= T; no++ ) {
		printf("Case #%d: ", no );
		test();
	}
	return 0;
}
