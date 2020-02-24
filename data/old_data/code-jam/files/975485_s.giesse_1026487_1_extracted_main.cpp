#include <cstdio>
#include <list>

using namespace std;

char table[9][9];
list<int> opposed[9];

int val(char c){
	//Each base element is a single character from {Q, W, E, R, A, S, D, F}
	if (c == 'Q') return 1;
	if (c == 'W') return 2;
	if (c == 'E') return 3;
	if (c == 'R') return 4;
	if (c == 'A') return 5;
	if (c == 'S') return 6;
	if (c == 'D') return 7;
	if (c == 'F') return 8;
	else return 0;
}

int main(){
	freopen("in.txt", "r", stdin);
	freopen("out.txt", "w", stdout);
	
	int t;
	scanf("%d", &t);
	for (int tc = 1; tc <= t; ++tc){
		int c,d,n;
		scanf("%d", &c);
		for (int i = 0; i < 9; ++i){
			for (int j = 0; j < 9; ++j) table[i][j] = -1;
		}
		for (int i = 0; i < c; ++i){
			char c1,c2,c3;
			scanf(" %c%c%c", &c1, &c2, &c3);
			table[val(c1)][val(c2)] = c3;
			table[val(c2)][val(c1)] = c3;
		}
		
		scanf("%d", &d);
		for (int i = 0; i < 9; ++i) opposed[i].clear();
		for (int i = 0; i < d; ++i){
			char c1,c2;
			scanf(" %c%c", &c1, &c2);
			opposed[val(c1)].push_back(val(c2));
			opposed[val(c2)].push_back(val(c1));
		}
		
		scanf("%d ", &n);
		char elems[1000];
		int bases[9];
		for (int i = 0; i < 9; ++i) bases[i] = 0;
		int count = 0;
		for (int i = 0; i < n; ++i){
			char c1;
			
			scanf("%c", &c1);
			bool changed = false;
			if (count > 0 && table[val(c1)][val(elems[count-1])] != -1){
				bases[val(elems[count-1])]--;
				elems[count-1] = table[val(c1)][val(elems[count-1])];
				changed = true;
			}
			if (!changed){
				elems[count] = c1;
				++count;
				bases[val(c1)]++;
			}
			for (list<int>::iterator it = opposed[val(elems[count-1])].begin(); 
			        it != opposed[val(elems[count-1])].end(); ++it){
				if (bases[*it] > 0){
					for (int j = 0; j < 9; ++j) bases[j] = 0;
					count = 0;
					break;
				}
			}
		}
		
		printf("Case #%d: [", tc);
		for (int i = 0; i < count-1; ++i){
			printf("%c, ", elems[i]);
		}
		if (count > 0)printf("%c]\n",elems[count-1]);
		else printf("]\n");
	}
	
	return 0;
}
