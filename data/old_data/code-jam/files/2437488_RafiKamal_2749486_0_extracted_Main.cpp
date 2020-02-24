#include <cstdio>
#include <cctype>
#include <algorithm>
#include <cstring>
#include <iostream>
#include <cmath>
#include <stack>
#include <string>     
#include <map>
#include <set>
#include <list>
#include <queue>
#include <utility>
#include <sstream>
#include <iomanip>
#include <fstream>

#define SIZE 105
#define SIZE2 220
#define MOD 20071027
#define clr(a) memset(a, 0, sizeof a)
#define reset(a) memset(a, -1, sizeof a)

#define BOUNDARY(i, j) ((i >= 0 && i < row) && (j >= 0 && j < column))

#define max3(x, y, z) max(max(x, y), max(y, z)) 

using namespace std;

int n, m, row, column, mx;
int x, y;
int t;

typedef pair<int, int> Parent;
struct Node
{
	int x;
	int y;
	int step;
	Node(int x, int y, int step){this->x = x; this->y = y; this->step = step;}
	Node(){}
};

int X[] = {0, 1, 0, -1};
int Y[] = {1, 0, -1, 0};
char d[] = {'N', 'E', 'S', 'W'};

bool visited[2*SIZE][2*SIZE][SIZE2];
Parent parent[2*SIZE][2*SIZE][SIZE2];
char dir[2*SIZE][2*SIZE][SIZE2];

void print(int x, int y, int step)
{
	if(step == 0)
		return;
	print(parent[x][y][step].first, parent[x][y][step].second, step - 1);
	putchar(dir[x][y][step]);
}	

int main()
{	
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
	int i, j, k, m, p, q, tc, x, y, a, b;
	
	scanf("%d", &tc);
	for(t = 1; t <= tc; t++)
	{
		scanf("%d %d", &x, &y);
		x += SIZE;
		y += SIZE;
		clr(visited);
		
		row = column = 2*SIZE;
		
		queue<Node> q;
		q.push(Node(SIZE, SIZE, 0));
		visited[SIZE][SIZE][0] = true;
		Node node;
		
		bool flag = true;
		if(x == SIZE && y == SIZE)
			flag = false;
		while(!q.empty() && flag) {
			node = q.front();
			q.pop();
			for(k = 0; k < 4; k++) {
				a = node.x + X[k]*(node.step+1);
				b = node.y + Y[k]*(node.step+1);
				if(BOUNDARY(a, b) && !visited[a][b][node.step+1]) {
					visited[a][b][node.step+1] = true;
					q.push(Node(a, b, node.step+1));
					parent[a][b][node.step+1] = make_pair(node.x, node.y);
					dir[a][b][node.step+1] = d[k];
					//printf("%d %d : %d %d - %d\n", a, b, node.x, node.y, node.step+1);
					if(x == a && y == b) {
						printf("Case #%d: ", t);
						flag = false;
						break;
					}
				}
			}
		}
		
		
		int stp = node.step + 1;
		if(x == SIZE && y == SIZE)
			stp = 0;
		print(a, b, stp);
		putchar('\n');
	}
			   
	return 0;
}