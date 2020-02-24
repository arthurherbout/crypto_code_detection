#include <cstdio>
#include <list>

using namespace std;

int abs(int a){
	return a < 0 ? -a : a;
}

struct task {
	int prio;
	int button;
	task(int p, int b) {prio = p; button = b; }
};

int main(){
	freopen("in.txt", "r", stdin);
	freopen("out.txt", "w", stdout);
	
	int t;
	scanf("%d", &t);
	for (int j = 1; j <= t; ++j){
		int n;
		scanf("%d", &n);
		list<task> blue;
		list<task> oran;
		for (int i = 0; i < n; ++i){
			char c;
			int b;
			scanf(" %c%d", &c,&b);
			if (c == 'B') blue.push_back(task(i,b));
			else if (c == 'O') oran.push_back(task(i,b));
		}
		int c = 0, o = 1, b = 1;
		for (int i = 0; i < n; ++i){
			if (oran.empty() || (!blue.empty() && blue.front().prio < oran.front().prio)){
				c = c+abs(blue.front().button-b)+1;
				if (abs(blue.front().button-b)+1 > abs(oran.front().button-o)) o = oran.front().button;
				else if (o < oran.front().button) o += abs(blue.front().button-b)+1;
				else if (o > oran.front().button) o -= abs(blue.front().button-b)+1;
				b = blue.front().button;
				blue.pop_front();
			}
			else if (blue.empty() || (!oran.empty() && oran.front().prio < blue.front().prio)){
				c = c+abs(oran.front().button-o)+1;
				if (abs(oran.front().button-o)+1 > abs(blue.front().button-b)) b = blue.front().button;
				else if (b < blue.front().button) b += abs(oran.front().button-o)+1;
				else if (b > blue.front().button) b -= abs(oran.front().button-o)+1;
				o = oran.front().button;
				oran.pop_front();
			}
		}
		
		printf("Case #%d: %d\n", j,c);
	}
	
	return 0;
}
