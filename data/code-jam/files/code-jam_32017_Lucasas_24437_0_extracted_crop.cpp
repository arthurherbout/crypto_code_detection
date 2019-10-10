#include <cstdio>
#include <vector>
using namespace std;

typedef unsigned long long ull;

struct coo {
	int x, y;
	coo(){
		x = 0; y = 0;
	}
	coo(int a, int b){
		x = a; y = b;
	}
};

int main(){
	freopen("crop.in", "r", stdin);
	int N;
	scanf("%d\n", &N);
	for (int casenr = 1; casenr <= N; casenr++){
		ull n, A, B, C, D, x0, y0, M;
		scanf("%llu %llu %llu %llu %llu %llu %llu %llu\n", &n, &A, &B, &C, &D, &x0, &y0, &M);
		vector<coo> list;
		list.push_back(coo(x0, y0));
		ull x=x0, y=y0;
		for (int i = 1; i < n; i++){
			x = (A*x+B)%M;
			y = (C*y+D)%M;
			list.push_back(coo(x, y));
		}
		int count = 0;
		for (int i = 0; i < list.size(); i++)
			for (int j = i+1; j < list.size(); j++)
				for (int k = j+1; k < list.size(); k++)
					if ((list[i].x+list[j].x+list[k].x)%3 == 0 &&
						(list[i].y+list[j].y+list[k].y)%3 == 0)
						count++;
		printf("Case #%d: %d\n", casenr, count);
	}	
	
}
