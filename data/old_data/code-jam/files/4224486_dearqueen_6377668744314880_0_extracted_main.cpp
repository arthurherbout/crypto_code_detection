#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>
using namespace std;

const int MAXN = 20;
const double Exp=1e-8;

void FileIO(){
    freopen("1.in", "r", stdin);
    freopen("1.out", "w", stdout);
}

struct Point{
	double x,y;
	int id;
}P;
inline double Det(Point p,Point p1,Point p2){
	return (p1.x-p.x)*(p2.y-p.y)-(p2.x-p.x)*(p1.y-p.y);
}
inline double Dis(Point p1,Point p2){
	return sqrt((p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y));
}
long cmp(Point a,Point b){
	int k=Det(P,a,b);
	return (k>0 || (k==0 && Dis(P,a)<Dis(P,b)));
}

struct Poly{
	Point point[MAXN];
	long stack[MAXN],top,cnt;
	bool onConvext[MAXN];

    void init(){
        memset(onConvext, false, sizeof(onConvext));
        top = 0;
        memset(stack, 0, sizeof(stack));
    }
	void Convext(){
	    init();
		for (long i=1;i<cnt;++i)
			if (point[i].y<point[0].y ||
				(point[i].y==point[0].y && point[i].x<point[0].x)){
					//Point tmp=point[0]; point[0]=point[i]; point[i]=tmp;
                    swap(point[i], point[0]);
                }

		P=point[0];
		std::sort(point+1,point+cnt,cmp);
		for (long i=0;i<=2;++i) stack[i]=i;
		top=2;

		for (long i=3;i<cnt;++i){
			while (top>=2 && Det(point[stack[top-1]],point[stack[top]],point[i])<0){
				top--;
			}
			stack[++top]=i;
			//onConvext[i] = true;
		}
		for (int i = 0; i <= top; ++i){
            onConvext[point[stack[i]].id] = true;
		}
	}

	double Circle_len(long n){
		double len=0; cnt=n;
		if (n==1) return 0.0;
		if (n==2) return Dis(point[0],point[1]);
		Convext();
		for (long i=1;i<=top;++i)
			len+=Dis(point[stack[i-1]],point[stack[i]]);
		len+=Dis(point[stack[top]],point[stack[0]]);
		return len;
	}
}tree, tmp;

int t, n, cut[MAXN];
Point p[MAXN];
bool vis[MAXN];

void judge(){
    int cnt = 0;
    for (int i = 0; i < n; ++i)
        if (vis[i]){
            tmp.point[cnt++] = tree.point[i];
        }
    tmp.cnt = cnt;
    if (cnt >= 3)
        tmp.Convext();

    for (int i = 0; i < n; ++i)
        if (tmp.onConvext[i])
            cut[i] = min(cut[i], n - cnt);
}

void dfs(int x){
    if (x == n){
        judge();
        return;
    }
    vis[x] = true;
    dfs(x + 1);
    vis[x] = false;
    dfs(x + 1);
}

int main(){
    FileIO();
    scanf("%d", &t);
    for (int k = 1; k <= t; ++k){
        scanf("%d", &n);
        tree.cnt = n;
        for (int i = 0; i < n; ++i){
            scanf("%lf%lf", &tree.point[i].x, &tree.point[i].y);
            tree.point[i].id = i;
        }
        memset(cut, 0x3f, sizeof(cut));

        dfs(0);
        printf("Case #%d:\n", k);
        for (int i = 0; i < n; ++i)
            printf("%d\n", cut[i]);
    }
    return 0;
}
